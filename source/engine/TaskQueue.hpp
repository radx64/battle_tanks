#pragma once

#include <functional>
#include <queue>
#include <mutex>

namespace engine
{

// TODO: add UT for this class.

class TaskQueue
{
public:
    void push(const std::function<void()>& task);
    void executeAll();

protected:
    std::queue<std::function<void()>> tasks;
    std::mutex mutex_;
};

}  // namespace engine
