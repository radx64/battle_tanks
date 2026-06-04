#pragma once

#include <sol/sol.hpp>

#include <SFML/System.hpp>

#include "engine/Logger.hpp"
#include "game/lua/WaitCondition.hpp"


namespace game::entity { class Tank; }

namespace game::lua
{
class ScriptContext 
{
public:

    // TODO: waypoints should not be part of script context, 
    // this is just a quick and dirty solution for PoC, refactor later
    ScriptContext(std::string_view scriptFile, entity::Tank* tank, std::vector<sf::Vector2i>& waypoints_);
    ~ScriptContext() = default;
        
    ScriptContext(const ScriptContext& other) = delete;
    ScriptContext(ScriptContext&& other) = default;
        
    ScriptContext& operator=(const ScriptContext& other) = delete;
    ScriptContext& operator=(ScriptContext&& other) = default;

    void reload();

    sol::coroutine& coroutine();
    std::unique_ptr<WaitCondition>& waitCondition();
    entity::Tank* tank();
    std::vector<sf::Vector2i>& waypoints();
    
    engine::Logger logger_;

protected:
    sol::state lua_state_;
    sol::coroutine coroutine_;
    entity::Tank* tank_;
    std::vector<sf::Vector2i>& waypoints_;
    std::unique_ptr<WaitCondition> wait_;
    const std::string_view scriptFile_;
};

}  // namespace game::lua
