#include "game/lua/ScriptContext.hpp"

#include "game/entity/tank/Tank.hpp"
#include "game/particle/Say.hpp"
#include "engine/Context.hpp"
#include "engine/ParticleSystem.hpp"
#include <cmath>
#include <algorithm>
#include <string>
#include "engine/math/Math.hpp"

namespace game::entity { class Tank; }

namespace game::lua::bindings
{
    
int lua_sleep(sol::this_state ts, double seconds);
int lua_set_turrent_heading(sol::this_state ts, double angle);
int lua_set_tank_heading(sol::this_state ts, double angle);
int lua_set_throttle(sol::this_state ts, double throttle);
int lua_print(sol::this_state ts);
sol::table lua_get_waypoints(sol::this_state ts);
sol::table lua_get_tank_position(sol::this_state ts);
int lua_fire_cannon(sol::this_state ts); // blocking if cannon is on cooldown, will yield until it can fire
int lua_move_to(sol::this_state ts, float x, float y); // blocking, drives to target then resumes
int lua_say(sol::this_state ts, const std::string& text);
}  // namespace game::lua::bindings


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


ScriptContext::ScriptContext(const std::string_view scriptFile, entity::Tank* tank, std::vector<sf::Vector2i>& waypoints)
: logger_{"ScriptContext"}
, tank_{tank}
, waypoints_{waypoints}
, scriptFile_{scriptFile}
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
    lua_state_.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table, sol::lib::string, sol::lib::coroutine);
    
    // Rebind functions
    lua_state_.set_function("sleep", game::lua::bindings::lua_sleep);
    lua_state_.set_function("set_turret_heading", game::lua::bindings::lua_set_turrent_heading);
    lua_state_.set_function("set_tank_heading", game::lua::bindings::lua_set_tank_heading);
    lua_state_.set_function("set_throttle", game::lua::bindings::lua_set_throttle);
    lua_state_.set_function("print", game::lua::bindings::lua_print);
    lua_state_.set_function("get_waypoints", game::lua::bindings::lua_get_waypoints);
    lua_state_.set_function("get_tank_position", game::lua::bindings::lua_get_tank_position);
    lua_state_.set_function("fire_cannon", game::lua::bindings::lua_fire_cannon);
    lua_state_.set_function("move_to", game::lua::bindings::lua_move_to);
    lua_state_.set_function("say", game::lua::bindings::lua_say);
    lua_state_.set_function("Say", game::lua::bindings::lua_say);
    // Reload the script
    lua_state_.script_file(std::string(scriptFile_));

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

std::vector<sf::Vector2i>& ScriptContext::waypoints()
{
    return waypoints_;
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

int lua_set_throttle(sol::this_state ts, double throttle)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());
    ctx->tank()->setThrottle(throttle);
    return 0;
}

sol::table lua_get_tank_position(sol::this_state ts)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());
    const auto& rb = ctx->tank()->getRigidBody();
    sol::table pos = lua.create_table_with("x", rb.x_, "y", rb.y_);
    return pos;
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

sol::table lua_get_waypoints(sol::this_state ts)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());
    
    sol::table waypoints = lua.create_table();
    const auto& wps = ctx->waypoints();
    for (size_t i = 0; i < wps.size(); ++i) {
        sol::table point = lua.create_table_with("x", wps[i].x, "y", wps[i].y);
        waypoints[i + 1] = point;
    }
    
    return waypoints;
}

int lua_fire_cannon(sol::this_state ts)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());

    if (!ctx->tank()->canFire()) {
        ctx->waitCondition() = std::make_unique<lua::WaitFireCycle>(ctx->tank());
        return lua_yield(lua, 0);
    }

    ctx->tank()->fire();
    return 0;
}

int lua_say(sol::this_state ts, const std::string& text)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());

    float x = 0.f;
    float y = 0.f;
    if (ctx && ctx->tank())
    {
        const auto& rb = ctx->tank()->getRigidBody();
        x = rb.x_;
        y = rb.y_;
    }

    auto say = std::make_unique<game::particle::Say>(text, x, y);
    engine::Context::getParticleSystem().add(std::move(say));
    return 0;
}

int lua_move_to(sol::this_state ts, float x, float y)
{
    sol::state_view lua(ts);
    ScriptContext* ctx = get_script_context(lua.lua_state());

    if (!ctx->tank()) return 0;

    const auto& rb = ctx->tank()->getRigidBody();
    float dx = x - rb.x_;
    float dy = y - rb.y_;
    float distance = std::sqrt(dx * dx + dy * dy);
    const float threshold = 30.0f;

    if (distance <= threshold) {
        ctx->tank()->setThrottle(0.0);
        return 0;
    }

    ctx->waitCondition() = std::make_unique<lua::WaitMoveTo>(ctx->tank(), x, y, threshold);
    return lua_yield(lua, 0);
}

}  // namespace game::lua::bindings
