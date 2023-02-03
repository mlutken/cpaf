#ifndef CPAF_TIME_H
#define CPAF_TIME_H

#include <string>
#include <ctime>
#include <chrono>
#include <boost/date_time/posix_time/posix_time.hpp>


namespace cpaf::time {

/** \unit cpaf_time.h
! Functions formatting ad parsing time.
\author Martin Lutken
*/


template <class Rep, class Period>
constexpr
std::chrono::duration<Rep, Period>
abs(std::chrono::duration<Rep, Period> d)
{
    const Rep x = d.count();
    return std::chrono::duration<Rep, Period>(x >= 0 ? x : -x);
}

// -----------------------------
// --- Types and Definitions ---
// -----------------------------

// ------------------------------
// --- Convenience formatters ---
// ------------------------------
constexpr std::string_view iso_pretty("%Y-%m-%d %H:%M:%S");
constexpr std::string_view iso_strict("%Y%m%dT%H%M%S");


// ---------------------------------
// --- Time formatting functions ---
// ---------------------------------
std::string format_local        (const std::time_t& raw_time, const std::string_view format_string);
std::string format_local        (const std::chrono::system_clock::time_point& time_point,
                                 const std::string_view format_string);
std::string format_local        (const boost::posix_time::ptime& time,
                                 const std::string_view format_string);
std::string format_local_pretty (const std::chrono::system_clock::time_point& time_point);
std::string now_local_pretty    ();

std::string format_utc          (const time_t& raw_time, const std::string_view format_string,
                                 bool append_utc = false);
std::string format_utc          (const std::chrono::system_clock::time_point& time_point,
                                 const std::string_view format_string, bool append_utc = false);
std::string format_utc          (const boost::posix_time::ptime& time,
                                 const std::string_view format_string, bool append_utc = false);
std::string format_utc_pretty   (const std::chrono::system_clock::time_point& time_point,
                                 bool append_utc = false);
std::string now_utc_pretty      (bool append_utc = false);


// ------------------------------
// --- Time parsing functions ---
// ------------------------------
std::chrono::system_clock::time_point parse_iso_pretty  (const std::string& date_time_string);

namespace test {
    std::string format_utc(const time_t& raw_time, const std::string_view format_string, bool append_utc = false);
}

// -------------------
// --- Converting ----
// -------------------
std::chrono::system_clock::time_point unix_epoch();

int64_t to_unix_timestamp_seconds  (const std::chrono::system_clock::time_point& time_point);
int64_t unix_timestamp_now_seconds ();

template <class Rep, class Period>
constexpr float to_float_seconds(std::chrono::duration<Rep, Period> duration)
{
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return static_cast<float>(static_cast<int>(ms * 10.0f)) / 10.0f;
}

// ---------------------------
// --- Timing measurements ---
// ---------------------------

/** Call function a specified number of times an return the average time
 * in seconds as a std::chrono::duration<double>

  @return Avarage duration in seconds (fractional, std::chrono::duration<double>)
*/
template <typename Function>
auto measure_function_call(std::size_t iter, Function&& f)
{
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < iter; ++i){
        f();
    }
    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> dur = end - start; // In seconds
    return dur / iter;
}


} //END namespace cpaf::time


#endif //CPAF_TIME_H


