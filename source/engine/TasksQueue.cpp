#include "engine/TasksQueue.hpp"

namespace engine
{

void TasksQueue::push(const std::function<void()>& task)
{
    tasks.push(task);
}

void TasksQueue::executeAll()
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
