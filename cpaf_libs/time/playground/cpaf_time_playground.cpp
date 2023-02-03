#include <iostream>
#include <vector>
#include <cpaf_libs/time/cpaf_time.h>

using namespace std;
using namespace cpaf::time;
using namespace boost::posix_time;

int main()
{
    cerr << "--- cpaf time playground ---\n";

    cerr << "std::time   : format_local('%A %c')            : " << format_local(std::time(nullptr), "%A %c") << "\n";
    cerr << "std::time   : format_local('%Y-%m-%d %H:%M:%S'): " << format_local(std::time(nullptr), "%Y-%m-%d %H:%M:%S") << "\n";
    cerr << "std::time   : format_utc('%Y-%m-%d %H:%M:%S')  : " << format_utc(std::time(nullptr), "%Y-%m-%d %H:%M:%S") << "\n";
    cerr << "std::chrono : format_local('%Y-%m-%d %H:%M:%S'): " << format_local(std::chrono::system_clock::now(), iso_pretty) << "\n";
    cerr << "std::chrono : format_utc('%Y-%m-%d %H:%M:%S')  : " << format_utc(std::chrono::system_clock::now(), iso_pretty) << "\n";
    cerr << "boost::posix: format_local('%Y-%m-%d %H:%M:%S'): " << format_local(second_clock::universal_time(), iso_pretty) << "\n";
    cerr << "boost::posix: format_utc('%Y-%m-%d %H:%M:%S')  : " << format_utc(second_clock::universal_time(), iso_pretty) << "\n";

    chrono::time_point tp = parse_iso_pretty("2020-12-03 12:24:17"); // 2020-11-30 13:38:09
    auto duration_fromepoch_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
    auto unix_timestamp = std::chrono::seconds(std::time(nullptr));
//    int unix_timestamp_x_1000 = std::chrono::milliseconds(unix_timestamp).count();

    cerr << "parse_iso_pretty         : " << tp.time_since_epoch().count() << "\n";
    cerr << "unix_timestamp_2         : " << duration_fromepoch_in_seconds.count() << "\n";
    cerr << "unix_timestamp_X         : " << 1606998257 << "\n";
    cerr << "to_unix_timestamp_seconds: " << to_unix_timestamp_seconds(tp) << "\n";

    cerr << "unix_timestamp: " << unix_timestamp.count() << "\n";


    return 0;
}
