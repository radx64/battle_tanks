#pragma once

#include <sol/sol.hpp>

#include "engine/Logger.hpp"
#include "game/lua/WaitCondition.hpp"


namespace game::entity { class Tank; }

namespace game::lua
{
class ScriptContext 
{
public:
    ScriptContext(entity::Tank* tank);
    ~ScriptContext() = default;
        
    ScriptContext(const ScriptContext& other) = delete;
    ScriptContext(ScriptContext&& other) = default;
        
    ScriptContext& operator=(const ScriptContext& other) = delete;
    ScriptContext& operator=(ScriptContext&& other) = default;

    void reload();

    sol::coroutine& coroutine();
    std::unique_ptr<WaitCondition>& waitCondition();
    entity::Tank* tank();
    
    engine::Logger logger_;

protected:
    sol::state lua_state_;
    sol::coroutine coroutine_;
    entity::Tank* tank_;
    std::unique_ptr<WaitCondition> wait_;
};

}  // namespace game::lua
