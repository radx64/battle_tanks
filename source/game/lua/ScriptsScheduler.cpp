#include "ScriptsScheduler.hpp"

#include <exception>

#include "ScriptContext.hpp"

namespace game::lua
{

void ScriptsScheduler::update(float dt)
{
    // iterate with iterator so we can erase completed scripts
    for (auto it = scripts_.begin(); it != scripts_.end();) {
        ScriptContext* script = *it;

        if (script->waitCondition()) {
            if (!script->waitCondition()->update(dt)) {
                ++it;
                continue;
            }
            script->waitCondition().reset();
        }

        resume(script);

        // if coroutine is no longer yielding it finished (or errored)
        if (script->coroutine().status() != sol::call_status::yielded) {
            it = scripts_.erase(it);
        } else {
            ++it;
        }
    }
}

bool ScriptsScheduler::hasActiveScripts() const noexcept
{
    return !scripts_.empty();
}

void ScriptsScheduler::resume(ScriptContext* ctx)
{
    if (!ctx->coroutine().lua_state()) {
        throw std::runtime_error("Attempted to resume script with invalid/null coroutine state");
    }
    auto result = ctx->coroutine()(ctx);
    if (!result.valid()) throw sol::error(result);  // TODO: add own exception type in later phase
}

void ScriptsScheduler::add(ScriptContext* ctx)
{
    if (!ctx->coroutine().lua_state()) {
        throw std::runtime_error("Adding script context without a valid coroutine");
    }
    scripts_.emplace_back(ctx);
}


}  // namespace game::lua
