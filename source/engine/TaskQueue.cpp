#include "engine/TaskQueue.hpp"

namespace engine
{

void TaskQueue::push(const std::function<void()>& task)
{
    tasks.push(task);
}

void TaskQueue::executeAll()
{
    std::lock_guard<std::mutex> lock(mutex_);
    while (!tasks.empty())
    {
        auto& task = tasks.front();
        task();
        tasks.pop();
    }
}

}  // namespace engine
