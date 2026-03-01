#pragma once

#include <sol/sol.hpp>

#include "WaitCondition.hpp"
#include "TankSimulator.hpp"

namespace experimental
{

struct ScriptContext 
{
    sol::coroutine coroutine;
    TankSimulator* tank;
    std::unique_ptr<WaitCondition> wait;

    ScriptContext() = default;
    ~ScriptContext() = default;
    ScriptContext(const sol::coroutine& co, TankSimulator* t, std::unique_ptr<WaitCondition> w) : coroutine(std::move(co)), tank(t), wait(std::move(w)) {}
        
    ScriptContext(const ScriptContext& other) = delete;
    ScriptContext(ScriptContext&& other) = default;
        
    ScriptContext& operator=(const ScriptContext& other) = delete;
    ScriptContext& operator=(ScriptContext&& other) = default;
};

}  // namespace experimental
