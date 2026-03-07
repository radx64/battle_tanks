#include <cstdint>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <sol/sol.hpp>

#include "TankSimulator.hpp"
#include "ScriptContext.hpp"
#include "ScriptScheduler.hpp"
#include "WaitCondition.hpp"

// LUA helpers and magic tricks
static const char* SCRIPT_PTR_KEY = "SCRIPT_CONTEXT_PTR";

// Store experimental::ScriptContext in hidden LUA_REGISTRYINDEX
void set_script_context(lua_State* L, experimental::ScriptContext* script)
{
    lua_pushlightuserdata(L, (void*)SCRIPT_PTR_KEY);
    lua_pushlightuserdata(L, script);
    lua_settable(L, LUA_REGISTRYINDEX);
}

experimental::ScriptContext* get_script_context(lua_State* L)
{
    lua_pushlightuserdata(L, (void*)SCRIPT_PTR_KEY);
    lua_gettable(L, LUA_REGISTRYINDEX);

    auto* script = static_cast<experimental::ScriptContext*>(lua_touserdata(L, -1));

    lua_pop(L, 1);
    return script;
}
//

double currentTime() 
{
    using namespace std::chrono;
    static auto start = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    return duration<double>(now - start).count();
}

// C++ sleep binding
// I have to calculate when to "wake up" script again
int lua_sleep(sol::this_state ts, double seconds) 
{
    sol::state_view lua(ts);
    experimental::ScriptContext* ctx = get_script_context(lua.lua_state());
    ctx->wait = std::make_unique<experimental::WaitSeconds>(seconds);
    return lua_yield(lua, 0);
}

// C++ turret rotation binding
// Need to set turret target in tank simluator
int lua_set_turret_heading(sol::this_state ts, double angle)
{
    std::cout << "Called set turret heading" << std::endl;
    sol::state_view lua(ts);
    
    experimental::ScriptContext* ctx = get_script_context(lua.lua_state());

    experimental::TankSimulator* tank = ctx->tank;
    tank->targetTurretHeading = angle;
    tank->isTurretRotating = true;

    ctx->wait = std::make_unique<experimental::WaitTurretAligned>(tank, angle);

    return lua_yield(lua, 0);
} 

// C++ turret rotation binding
// Need to set tank body heading target in tank simluator
int lua_set_tank_heading(sol::this_state ts, double angle)
{
    std::cout << "Called set tank heading" << std::endl;
    sol::state_view lua(ts);
    
    experimental::ScriptContext* ctx = get_script_context(lua.lua_state());

    experimental::TankSimulator* tank = ctx->tank;

    tank->targetBodyHeading = angle;
    tank->isBodyRotating = true;

    ctx->wait = std::make_unique<experimental::WaitBodyAligned>(tank, angle);

    return lua_yield(lua, 0);
} 

int lua_fire(sol::this_state ts)
{
    (void) ts;
    std::cout << "*** FIRE! ***" << std::endl;
    return 0;
}

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::coroutine);

    experimental::ScriptsScheduler scheduler;
    experimental::TankSimulator tank;
    experimental::ScriptContext ctx;
    ctx.tank = &tank;
    
    lua.set_function("sleep", lua_sleep);
    lua.set_function("set_turret_heading", lua_set_turret_heading);
    lua.set_function("set_tank_heading", lua_set_tank_heading);
    lua.set_function("fire", lua_fire);

    lua.script_file("scripts/program.lua");

    // Create coroutine
    sol::protected_function mainFunc = lua["main"];
    sol::coroutine co(mainFunc);

    // save coroutine to context so scheduler can resume it later
    ctx.coroutine = std::move(co);

    // make the script context available in the coroutine's registry
    set_script_context(ctx.coroutine.lua_state(), &ctx);

    // Start coroutine immediately so we can detect if it yields or finishes
    auto result = ctx.coroutine();
    if (!result.valid()) {
        sol::error err = result;
        std::cout << "Lua error: " << err.what() << std::endl;
    }

    // only track the coroutine if it yielded (meaning it isn't finished yet)
    if (result.status() == sol::call_status::yielded) {
        scheduler.add(&ctx);
    } // if it finished right away we don't need to put it in scheduler

    // -------- GAME LOOP --------
    double last = currentTime();

    // continue until scheduler has no more active scripts
    while (scheduler.hasActiveScripts())
    {
        double now = currentTime();
        double dt = now - last;
        last = now;

        tank.update(dt);
        scheduler.update(dt);

        // simulate game loop other tasks (physics, drawing, etc)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cout << "Final tank turret heading: " << tank.turretHeading << std::endl;
    std::cout << "Final tank body heading: " << tank.bodyHeading << std::endl;

    return 0;
}
