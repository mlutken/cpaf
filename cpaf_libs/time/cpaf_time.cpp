#include "cpaf_time.h"

#include <sstream>
#include <iomanip>
#include <chrono>
#include <boost/date_time/posix_time/conversion.hpp>


namespace cpaf::time {

// ---------------------------------
// --- Time formatting functions ---
// ---------------------------------

std::string format_local(const time_t& raw_time,
                         const std::string_view format_string)
{
    std::stringstream ss;
    ss << std::put_time(std::localtime(&raw_time), format_string.data());
    return ss.str();
}

std::string format_local(const std::chrono::system_clock::time_point& time_point,
                         const std::string_view format_string)
{
    return format_local(std::chrono::system_clock::to_time_t(time_point), format_string);
}

std::string format_local(const boost::posix_time::ptime& time,
                         const std::string_view format_string)
{
    return format_local(boost::posix_time::to_time_t(time), format_string);
}

std::string format_local_pretty(const std::chrono::system_clock::time_point& time_point)
{
    return format_local(time_point, iso_pretty);
}

std::string now_local_pretty()
{
    return format_local_pretty(std::chrono::system_clock::now());
}



std::string format_utc(const time_t& raw_time,
                       const std::string_view format_string,
                       bool append_utc)
{
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&raw_time), format_string.data());
    return append_utc ? ss.str() + " UTC" : ss.str();
}

std::string format_utc(const std::chrono::system_clock::time_point& time_point,
                       const std::string_view format_string,
                       bool append_utc)
{
    return format_utc(std::chrono::system_clock::to_time_t(time_point), format_string, append_utc);
}

std::string format_utc(const boost::posix_time::ptime& time,
                       const std::string_view format_string,
                       bool append_utc)
{
    return format_utc(boost::posix_time::to_time_t(time), format_string, append_utc);
}

std::string format_utc_pretty(const std::chrono::system_clock::time_point& time_point,
                              bool append_utc)
{
    return format_utc(time_point, iso_pretty, append_utc);
}

std::string now_utc_pretty(bool append_utc)
{
    return format_utc_pretty(std::chrono::system_clock::now(), append_utc);
}

// Fo testing the speed of the strftime version againts the put_time version
namespace test {
    std::string format_utc(const time_t& raw_time,
                           const std::string_view format_string,
                           bool append_utc)
    {
        char buffer[100];
        if (!std::strftime(buffer, sizeof(buffer), format_string.data(), std::gmtime(&raw_time))) {
            return "";
        }
        return append_utc ? std::string(buffer) + " UTC" : buffer;
    }
}

// ------------------------------
// --- Time parsing functions ---
// ------------------------------

std::chrono::system_clock::time_point parse_iso_pretty(const std::string& date_time_string)
{
    std::tm tm = {};
    std::stringstream ss(date_time_string);
    ss >> std::get_time(&tm, std::string(iso_pretty).c_str());
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return tp;
}

// -------------------
// --- Converting ----
// -------------------

std::chrono::system_clock::time_point unix_epoch()
{
    static bool initialized = false;
    static std::chrono::system_clock::time_point epoch;
    if (!initialized) {
        epoch = parse_iso_pretty ("1970-01-01 00:00:00");
        initialized = true;
    }
    return epoch;
}



/** Convert to a standard unix timestamp with seconds resolution. */
int64_t to_unix_timestamp_seconds(const std::chrono::system_clock::time_point& time_point)
{
    const auto duration_from_epoch_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(time_point - unix_epoch());
    return duration_from_epoch_in_seconds.count();
}

int64_t unix_timestamp_now_seconds()
{
    return to_unix_timestamp_seconds(std::chrono::system_clock::now());
}

// --------------------------------------------------------------------------
// --- Convenience functions. Can freely use platform dependant functions ---
// --------------------------------------------------------------------------


} //END namespace cpaf::time
