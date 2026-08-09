#pragma once

#include "engine/Profiler.hpp"

namespace engine
{

class ProfilingStorage
{
public:
    void store(const ProfileResult& result);
    const std::vector<ProfileResult>& get() const;
    void reset();

protected:
    std::vector<ProfileResult> results_;
};

} // namespace engine
