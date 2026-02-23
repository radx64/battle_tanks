#include "engine/ProfilingStorage.hpp"

namespace engine
{

void ProfilingStorage::store(const ProfileResult& result)
{
    results_.push_back(result);
}

const std::vector<ProfileResult>& ProfilingStorage::get() const
{
    return results_;
}

void ProfilingStorage::reset()
{
    results_.clear();
}

} // namespace engine
