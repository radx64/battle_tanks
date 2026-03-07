#pragma once

#include <vector>

#include "ScriptContext.hpp"

namespace game::lua
{
class ScriptsScheduler
{
public:
    void update(float dt);
    bool hasActiveScripts() const noexcept;
    void resume(ScriptContext* ctx);
    void add(ScriptContext* ctx);

protected:
    std::vector<ScriptContext*> scripts_; // TODO: think who should own ctx, tank?
};

}  // namespace game::lua
