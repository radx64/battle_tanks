#include "ScriptsScheduler.hpp"

#include <algorithm>
#include <exception>
#include <string>

#include "ScriptContext.hpp"

namespace game::lua
{

void ScriptsScheduler::update(float dt)
{
    // iterate with iterator so we can erase completed scripts
    for (auto it = scripts_.begin(); it != scripts_.end();) {
        ScriptContext* script = *it;

        if (script->isStopped()) {
            it = scripts_.erase(it);
            continue;
        }

        if (script->waitCondition()) {
            if (!script->waitCondition()->update(dt)) {
                ++it;
                continue;
            }
            script->waitCondition().reset();
        }

        resume(script);

        // if coroutine is no longer yielding it finished (or errored)
        if (script->isStopped() || script->coroutine().status() != sol::call_status::yielded) {
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
    if (ctx->isStopped()) {
        return;
    }

    if (!ctx->coroutine().lua_state()) {
        ctx->stop("Attempted to resume script with invalid/null coroutine state");
        return;
    }

    try {
        auto result = ctx->coroutine()(ctx);
        if (!result.valid()) {
            sol::error error(result);
            ctx->stop(std::string("Lua runtime error: ") + error.what());
        }
    }
    catch (const sol::error& e) {
        ctx->stop(std::string("Lua runtime error: ") + e.what());
    }
    catch (const std::exception& e) {
        ctx->stop(std::string("Script runtime error: ") + e.what());
    }
    catch (...) {
        ctx->stop("Unknown script runtime error");
    }
}

void ScriptsScheduler::add(ScriptContext* ctx)
{
    if (ctx->isStopped()) {
        return;
    }

    if (!ctx->coroutine().lua_state()) {
        throw std::runtime_error("Adding script context without a valid coroutine");
    }

    if (std::find(scripts_.begin(), scripts_.end(), ctx) != scripts_.end()) {
        return;
    }

    scripts_.emplace_back(ctx);
}


}  // namespace game::lua
