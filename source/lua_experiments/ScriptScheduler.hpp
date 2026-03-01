#pragma once

#include <exception>
#include <vector>

#include "ScriptContext.hpp"

namespace experimental
{
class ScriptScheduler
{
public:
    void update(float dt)
    {
        // iterate with iterator so we can erase completed scripts
        for (auto it = scripts_.begin(); it != scripts_.end();) {
            ScriptContext* script = *it;

            if (script->wait) {
                if (!script->wait->update(dt)) {
                    ++it;
                    continue;
                }
                script->wait.reset();
            }

            resume(script);

            // if coroutine is no longer yielding it finished (or errored)
            if (script->coroutine.status() != sol::call_status::yielded) {
                it = scripts_.erase(it);
            } else {
                ++it;
            }
        }
    }

    bool hasActiveScripts() const noexcept
    {
        return !scripts_.empty();
    }
    
    void resume(ScriptContext* ctx)
    {
        if (!ctx->coroutine.lua_state()) {
            throw std::runtime_error("Attempted to resume script with invalid/null coroutine state");
        }
        auto result = ctx->coroutine(ctx);
        if (!result.valid()) throw sol::error(result);  // TODO: add own exception type in later phase
    }

    void add(ScriptContext* ctx)
    {
        if (!ctx->coroutine.lua_state()) {
            throw std::runtime_error("Adding script context without a valid coroutine");
        }
        scripts_.emplace_back(ctx);
    }

protected:
    std::vector<ScriptContext*> scripts_; // TODO: think who should own ctx
};

}  // namespace experimental
