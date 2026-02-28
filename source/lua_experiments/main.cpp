#include <cstdint>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <sol/sol.hpp>

struct TankSimulator 
{
    float targetTurretHeading = 0.f;
    float turretHeading = 0.f;
    const float turretRotationSpeed = 300.f;
    bool isTurretRotating = false;

    float targetBodyHeading = 0.f;
    float bodyHeading = 0.f;
    const float bodyRotationSpeed = 100.f;
    bool isBodyRotating = false;

    void processRotation(auto& elementHeading, auto& elementHeadingTarget, auto& isElementRotating, const auto elementRotationSpeed, float dt, std::string_view elementName)
    {
        if (not isElementRotating)
        {
            return;
        }

        std::cout << "Rotating " << elementName << " current: " << elementHeading << " target: " << elementHeadingTarget << std::endl;
        
        float diff = elementHeadingTarget - elementHeading;
        float step = elementRotationSpeed * dt;
        if (std::abs(diff) <= step) 
        {
            elementHeading = elementHeadingTarget;
            isElementRotating = false;
        } 
        else 
        {
            elementHeading += (diff > 0 ? step : -step);
        }
    }

    void update(float dt)
    {
        processRotation(turretHeading, targetTurretHeading, isTurretRotating, turretRotationSpeed, dt, "turret");
        processRotation(bodyHeading, targetBodyHeading, isBodyRotating, bodyRotationSpeed, dt, "body");
    }

};

struct ScriptContext 
{
    TankSimulator* tank = nullptr;  // Context will now own a tank
    double wakeTime = 1e30;         // for sleep
    bool waiting = false;           // has coroutine finished or waiting for turret, body movement finished
};

// LUA helpers and magic tricks
static const char* SCRIPT_PTR_KEY = "SCRIPT_CONTEXT_PTR";

// Store ScriptContext in hidden LUA_REGISTRYINDEX
void set_script_context(lua_State* L, ScriptContext* script)
{
    lua_pushlightuserdata(L, (void*)SCRIPT_PTR_KEY);
    lua_pushlightuserdata(L, script);
    lua_settable(L, LUA_REGISTRYINDEX);
}

ScriptContext* get_script_context(lua_State* L)
{
    lua_pushlightuserdata(L, (void*)SCRIPT_PTR_KEY);
    lua_gettable(L, LUA_REGISTRYINDEX);

    auto* script = static_cast<ScriptContext*>(lua_touserdata(L, -1));

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
    
    ScriptContext* script = get_script_context(lua.lua_state());
    script->wakeTime = currentTime() + seconds;

    return lua_yield(lua, 0);
}

// C++ turret rotation binding
// Need to set turret target in tank simluator
int lua_set_turret_heading(sol::this_state ts, double angle)
{
    std::cout << "Called set turret heading" << std::endl;
    sol::state_view lua(ts);
    
    ScriptContext* script = get_script_context(lua.lua_state());

    TankSimulator* tank = script->tank;

    tank->targetTurretHeading = angle;
    tank->isTurretRotating = true;
    script->waiting = true;

    return lua_yield(lua, 0);
} 

// C++ turret rotation binding
// Need to set tank body heading target in tank simluator
int lua_set_tank_heading(sol::this_state ts, double angle)
{
    std::cout << "Called set tank heading" << std::endl;
    sol::state_view lua(ts);
    
    ScriptContext* script = get_script_context(lua.lua_state());

    TankSimulator* tank = script->tank;

    tank->targetBodyHeading = angle;
    tank->isBodyRotating = true;
    script->waiting = true;

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

    TankSimulator tank;
    ScriptContext ctx;
    ctx.tank = &tank;

    lua.set_function("sleep", lua_sleep);
    lua.set_function("set_turret_heading", lua_set_turret_heading);
    lua.set_function("set_tank_heading", lua_set_tank_heading);
    lua.set_function("fire", lua_fire);

    lua.script_file("scripts/program.lua");

    // Create coroutine
    sol::protected_function mainFunc = lua["main"];
    sol::coroutine co(mainFunc);

    set_script_context(co.lua_state(), &ctx);

    // Start coroutine
    auto result = co();
    if (!result.valid()) {
        sol::error err = result;
        std::cout << "Lua error: " << err.what() << std::endl;
    } // lua can hide error if function aruments does not match (it took me like an hour to figure that one)

    // -------- GAME LOOP --------
    double last = currentTime();

    while (co.status() != sol::call_status::ok || ctx.waiting) 
    {
        double now = currentTime();
        double dt = now - last;
        last = now;

        tank.update(dt);

        bool readyFromSleep =  ctx.wakeTime < 1e29 && now >= ctx.wakeTime;
        bool readyFromTurret = ctx.waiting && !tank.isTurretRotating;
        bool readyFromBody = ctx.waiting && !tank.isBodyRotating;

        if (co.status() == sol::call_status::yielded &&
            (readyFromSleep || readyFromTurret || readyFromBody))  // this will require some kind of waiting conditions implementation
        {
            ctx.waiting = false;
            ctx.wakeTime = 1e30;

            co(); // resume coruotine
            std::cout << "Coroutine status: " << (int)co.status() << std::endl;

            // TODO: coroutine yelds and normal returns mixes are not working properly right now, 
            // if i call fire() in lua it will end up immidately
 
        }

        // simluate game loop other tasks (physics, drawing, etc)
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cout << "Final tank turret heading: " << tank.turretHeading << std::endl;
    std::cout << "Final tank body heading: " << tank.bodyHeading << std::endl;

    return 0;
}
