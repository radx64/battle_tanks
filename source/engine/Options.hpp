#pragma once

#include <bitset>
#include <cassert>

namespace engine
{

template <typename T>
class Option
{
public:
    Option(const bool value, const T position)
    {
        assert(static_cast<size_t>(position) != static_cast<size_t>(T::Last) &&
            "Can't use <T>::Last as an option");
        option_ = value << static_cast<size_t>(position);
    }

    size_t get() const
    {
        return option_;
    }

protected:
    size_t option_;
};

template <typename T>
class Options
{
public:
    Options()
    : bits_{}
    {
    }

    Options(const size_t raw_bits)
    : bits_{raw_bits}
    {
    }

    Options(const std::initializer_list<Option<T>>& options)
    {
        size_t raw_bits{};

        for (const auto& option : options)
        {
            raw_bits += option.get();
        }

        bits_ = {raw_bits};
    }

    bool isSet(const T& option) const
    {
        return bits_.test(static_cast<size_t>(option));
    }

    void set(const T& option)
    {
        bits_.set(static_cast<size_t>(option));
    }

protected:
    std::bitset<static_cast<size_t>(T::Last)> bits_;
};

}  // namespace engine
