#pragma once

#include <chrono>
#include <thread>
#include <atomic>

namespace estl {


/**
Wait until variable reaches expected value.
    @param wait_on Const reference to the (typically atomic) variable that we
           need to wait for until another thread changes it's valu.
    @param wait_for_value The value we are waiting for the @a wait_for to reach (changed
            by another thread).
        @param max_wait_time The maximim time to wait before bailing out.
        @param loop_yield_time The time to yield/sleep in each loop iteration.

@return True if value changed before timeout, false otherwise.
*/
template <typename T>
bool wait_for_expected_value (
    const T& wait_on,
    T wait_for_value,
    std::chrono::nanoseconds max_wait_time,
    std::chrono::nanoseconds loop_yield_time = std::chrono::milliseconds(1)
    )
{
    const auto ts_end = std::chrono::steady_clock::now() + max_wait_time;
    while (!(wait_on == wait_for_value) && (std::chrono::steady_clock::now() < ts_end))
    {
        std::this_thread::sleep_for(loop_yield_time);
    }
    return std::chrono::steady_clock::now() < ts_end;
}


template <typename T>
bool wait_for_expected_value (
    const std::atomic<T>& wait_on,
    T wait_for_value,
    std::chrono::nanoseconds max_wait_time,
    std::chrono::nanoseconds loop_yield_time = std::chrono::milliseconds(1)
    )
{
    const auto ts_end = std::chrono::steady_clock::now() + max_wait_time;
    while (!(wait_on == wait_for_value) && (std::chrono::steady_clock::now() < ts_end))
    {
        std::this_thread::sleep_for(loop_yield_time);
    }
    return std::chrono::steady_clock::now() < ts_end;
}


/**
Wait until function call reaches expected value.
    @param wait_on Const reference to the (typically atomic) variable that we
           need to wait for until another thread changes it's valu.
    @param wait_for_value The value we are waiting for the @a wait_for to reach (changed
            by another thread).
        @param max_wait_time The maximim time to wait before bailing out.
        @param loop_yield_time The time to yield/sleep in each loop iteration.

@return True if value changed before timeout, false otherwise.
*/
template <class CALLABLE, typename T>
bool wait_for_expected (
    CALLABLE& wait_on_function,
    T wait_for_value,
    std::chrono::nanoseconds max_wait_time,
    std::chrono::nanoseconds loop_yield_time = std::chrono::milliseconds(1)
    )
{
    const auto ts_end = std::chrono::steady_clock::now() + max_wait_time;
    while (!(wait_on_function() == wait_for_value) && (std::chrono::steady_clock::now() < ts_end))
    {
        std::this_thread::sleep_for(loop_yield_time);
    }
    return std::chrono::steady_clock::now() < ts_end;
}

} // END namespace estl
