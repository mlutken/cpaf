#pragma once

#include <string>
#include <ctime>
#include <chrono>
#include <format>
#include <optional>
#include <boost/date_time/posix_time/posix_time.hpp>


namespace cpaf::time {

enum class unit {unknown, years, months, weeks, days, hours, minutes, seconds, milliseconds, microseconds, nanoseconds};


/** \unit cpaf_time.h
! Functions formatting ad parsing time.
\author Martin Lutken
*/

/**
 * @brief Split a chrono::duration into durations
 * @param dur Duration to split into durations
 * @return tuple<Durations...> - A tuple of the durations to splint into
 * @example
 * auto dur = get_my_duration()...
 * auto [h,m,s] = duration_split<hours, minutes, seconds>(dur);
 * @endexample
 * @see https://stackoverflow.com/questions/7230621/how-can-i-iterate-over-a-packed-variadic-template-argument-list
 */
template<class... Durations, class DurationIn>
std::tuple<Durations...> duration_split( DurationIn dur ) {
    std::tuple<Durations...> retval;
    using discard=int[];
    (void)discard{0,(void((
                           (std::get<Durations>(retval) = std::chrono::duration_cast<Durations>(dur)),
                           (dur -= std::chrono::duration_cast<DurationIn>(std::get<Durations>(retval)))
                           )),0)...};
    return retval;
}

// -------------------------------------
// --- Duration formatting functions ---
// -------------------------------------
/** Format duration as "H:mm:ss" */
template <class Rep, class Period>
constexpr std::string
format_h_m_s(std::chrono::duration<Rep, Period> dur) {
    using namespace std::chrono;
    auto [h,m,s] = duration_split<hours, minutes, seconds>(dur);
    return std::format("{}:{:02}:{:02}", h.count(), m.count(), s.count());
}

/** Format duration as "H:mm:ss,xxx" */
template <class Rep, class Period>
constexpr std::string
format_h_m_s_ms(std::chrono::duration<Rep, Period> dur) {
    using namespace std::chrono;
    auto [h,m,s, ms] = duration_split<hours, minutes, seconds, milliseconds>(dur);
    return std::format("{}:{:02}:{:02},{:03}", h.count(), m.count(), s.count(), ms.count());
}



template <class Rep, class Period>
constexpr std::chrono::duration<Rep, Period>
abs(std::chrono::duration<Rep, Period> d)
{
    const Rep x = d.count();
    return std::chrono::duration<Rep, Period>(x >= 0 ? x : -x);
}

template <class Rep, class Period>
constexpr std::chrono::duration<Rep, Period>
invalid_duration()
{
    return std::chrono::duration<Rep, Period>::max();
}

template <class Rep, class Period>
constexpr bool
is_valid(std::chrono::duration<Rep, Period> d)
{
    return d == invalid_duration<Rep,Period>();
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
std::string steady_now_h_m_s    ();
std::string steady_now_h_m_s_ms ();


// ------------------------------
// --- Time parsing functions ---
// ------------------------------
std::chrono::system_clock::time_point   parse_iso_pretty    (const std::string& date_time_string);

/** Get duration from string with unit.
 *  Parses string like: '3d', '3h', '23m', '2s', '2 s', '450 ms', '50us', '23ns'
 *  @todo Only seconds supported in current version!
*/
std::optional<std::chrono::nanoseconds> parse_duration               (const std::string& duration);

/** Get duration from string with unit.
 *  Parses string like: '3d', '3h', '23m', '2s', '2 s', '450 ms', '50us', '23ns'
 *  @todo Only seconds supported in current version!
*/
std::chrono::nanoseconds                parse_duration               (const std::string& duration, std::chrono::nanoseconds default_value);

/** Parse time unit from string .
 *  Parses string like: 'd', 'days', 'h', 'hours', 's', 'seconds', 'milliseconds', 'ms', 'us', ns'
 *  @todo Only seconds supported in current version!
*/
unit                                    parse_unit          (std::string::const_iterator& begin
                                                            ,const std::string::const_iterator& end);

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

