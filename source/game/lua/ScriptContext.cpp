#include "game/lua/ScriptContext.hpp"

#include "game/entity/tank/Tank.hpp"

namespace game::entity { class Tank; }

namespace game::lua::bindings
{
    
int lua_sleep(sol::this_state ts, double seconds);
int lua_set_turrent_heading(sol::this_state ts, double angle);
int lua_set_tank_heading(sol::this_state ts, double angle);
int lua_print(sol::this_state ts);

}  // namespace game::lua:bindings


namespace game::lua
{

namespace 
{

// LUA helpers and magic tricks
static const char* SCRIPT_PTR_KEY = "SCRIPT_CONTEXT_PTR";

// Store ScriptContext in hidden LUA_REGISTRYINDEX
void storeScriptContext(lua_State* L, ScriptContext* script)
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

}  // namespace


ScriptContext::ScriptContext(entity::Tank* tank)
: logger_{"ScriptContext"}
, tank_{tank}
{
    reload();
}

void ScriptContext::reload()
{
    logger_.info("Reloading script...");
    
    // Clear the old coroutine first
    coroutine_ = sol::coroutine();
    
    // Now clear and reinitialize the lua state
    lua_state_ = sol::state();
    lua_state_.open_libraries(sol::lib::base, sol::lib::coroutine);
    
    // Rebind functions
    lua_state_.set_function("sleep", game::lua::bindings::lua_sleep);
    lua_state_.set_function("set_turret_heading", game::lua::bindings::lua_set_turrent_heading);
    lua_state_.set_function("set_tank_heading", game::lua::bindings::lua_set_tank_heading);
    lua_state_.set_function("print", game::lua::bindings::lua_print);

    // Reload the script
    lua_state_.script_file("scripts/tank_demo.lua");

    // Create fresh coroutine
    sol::coroutine co(lua_state_["main"]);
    coroutine_ = std::move(co);
    wait_.reset();
    
    // Store context pointer after everything is set up
    storeScriptContext(coroutine_.lua_state(), this);
}

sol::coroutine& ScriptContext::coroutine()
{
    return coroutine_;
}

std::unique_ptr<WaitCondition>& ScriptContext::waitCondition()
{
    return wait_;
}

entity::Tank* ScriptContext::tank()
{
    return tank_;
}

}  // namespace game::lua

namespace game::lua::bindings
{

int lua_sleep(sol::this_state ts, double seconds)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());
    ctx->waitCondition() = std::make_unique<lua::WaitSeconds>(seconds);
    return lua_yield(lua, 0);
}

int lua_set_turrent_heading(sol::this_state ts, double angle)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());
    ctx->tank()->setTurretHeading(angle);
    return 0;
}

int lua_set_tank_heading(sol::this_state ts, double angle)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());
    ctx->tank()->setHeading(angle);
    return 0;
}

int lua_print(sol::this_state ts)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());
    
    // Get all arguments passed to print
    std::string output;
    int nargs = lua_gettop(lua.lua_state());
    
    for (int i = 1; i <= nargs; i++) {
        if (!output.empty()) output += "\t";
        
        // Convert each argument to string
        lua_getglobal(lua.lua_state(), "tostring");
        lua_pushvalue(lua.lua_state(), i);
        lua_call(lua.lua_state(), 1, 1);
        output += lua_tostring(lua.lua_state(), -1);
        lua_pop(lua.lua_state(), 1);
    }
    
    // Log using the script context's logger
    ctx->logger_.info(output);
    return 0;
}

}  // namespace game::lua::bindings
