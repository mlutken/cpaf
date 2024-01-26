#include "cpaf_u16string_parse_numbers.h"

#include <algorithm>
#include <utils/cpaf_string_utils.hpp>

#include "cpaf_u16string_utils.h"

namespace cpaf::unicode {

void normalize_locale(std::u16string::iterator begin,
                             const std::u16string::iterator& end,
                             const char16_t decimal_point,
                             const char16_t thousands_separator)
{
    for ( ; begin != end; ++begin) {
        char16_t& c = *begin;
        if (c == decimal_point) {
            c = chars16::dot;
        }
        else if (c == thousands_separator) {
            c = chars16::comma;
        }
    }
}

bool parse_double ( double& value,
                    const std::u16string::const_iterator& begin,
                    const std::u16string::const_iterator& end,
                    double default_value,
                    const char16_t decimal_point,
                    const char16_t thousands_separator
                   ) noexcept
{
    std::string str = to_string(begin, end);
    std::string dp = to_string(decimal_point);
    std::string ts = to_string(thousands_separator);
    return cpaf::parse_double(value, str.begin(), str.end(), default_value, dp, ts);
}

bool parse_double ( double& value,
                    const std::u16string& sString,
                    double default_value,
                    const char16_t decimal_point,
                    const char16_t thousands_separator) noexcept
{
    return parse_double (value, sString.begin(), sString.end(), default_value, decimal_point, thousands_separator);
}


double parse_double 	( std::u16string::const_iterator &begin,
                          const std::u16string::const_iterator& end,
                          double default_value,
                          const char16_t decimal_point,
                          const char16_t thousands_separator) noexcept
{
    double value = 0;
    if (!parse_double (value, begin, end, default_value, decimal_point, thousands_separator)) {
        value = default_value;
    }
    return value;
}

double  parse_double 	( const std::u16string::const_iterator& begin,
                          const std::u16string::const_iterator& end,
                          double default_value,
                          const char16_t decimal_point,
                          const char16_t thousands_separator) noexcept
{
    std::u16string::const_iterator begin_mutable = begin;
    return parse_double (begin_mutable, end, default_value, decimal_point, thousands_separator);
}

double  parse_double 	( const std::u16string& sString,
                          double default_value,
                          const char16_t decimal_point,
                          const char16_t thousands_separator) noexcept
{
    return parse_double (sString.begin(), sString.end(), default_value, decimal_point, thousands_separator);
}


bool parse_float (float& value,
                  const std::u16string::const_iterator& begin,
                  const std::u16string::const_iterator& end,
                  float default_value,
                  const char16_t decimal_point,
                  const char16_t thousands_separator) noexcept
{
    std::string str = to_string(begin, end);
    std::string dp = to_string(decimal_point);
    std::string ts = to_string(thousands_separator);
    return cpaf::parse_float(value, str.begin(), str.end(), default_value, dp, ts);
}

bool parse_float (float& value,
                  const std::u16string& sString,
                  float default_value,
                  const char16_t decimal_point,
                  const char16_t thousands_separator) noexcept
{
    return parse_float (value, sString.begin(), sString.end(), default_value, decimal_point, thousands_separator);
}

float parse_float 	( std::u16string::const_iterator &begin,
                      const std::u16string::const_iterator& end,
                      float default_value,
                      const char16_t decimal_point,
                      const char16_t thousands_separator) noexcept
{
    float value = 0;
    if (!parse_float (value, begin, end, default_value, decimal_point, thousands_separator)) {
        value = default_value;
    }
    return value;
}

float  parse_float 	( const std::u16string::const_iterator& begin,
                          const std::u16string::const_iterator& end,
                          float default_value,
                          const char16_t decimal_point,
                          const char16_t thousands_separator) noexcept
{
    std::u16string::const_iterator begin_mutable = begin;
    return parse_float (begin_mutable, end, default_value, decimal_point, thousands_separator);
}

float  parse_float 	( const std::u16string& sString,
                          float default_value,
                          const char16_t decimal_point,
                          const char16_t thousands_separator) noexcept
{
    return parse_float (sString.begin(), sString.end(), default_value, decimal_point, thousands_separator);
}




bool parse_long (long& value,
                  const std::u16string::const_iterator& begin,
                  const std::u16string::const_iterator& end,
                  long default_value,
                  const char16_t thousands_separator,
                  int iBase
               ) noexcept
{
    std::string str = to_string(begin, end);
    std::string ts = to_string(thousands_separator);
    return cpaf::parse_long(value, str.begin(), str.end(), default_value, ts, iBase);
}

bool parse_long (long& value,
                  const std::u16string& sString,
                  long default_value,
                  const char16_t thousands_separator,
                  int iBase
              ) noexcept
{
    return parse_long (value, sString.begin(), sString.end(), default_value, thousands_separator, iBase);
}


long parse_long 	( std::u16string::const_iterator &begin,
                      const std::u16string::const_iterator& end,
                      long default_value,
                      const char16_t thousands_separator,
                      int iBase
                    ) noexcept
{
    long value;
    parse_long(value, begin, end, default_value, thousands_separator, iBase);
    return value;
}

long 	parse_long 	( const std::u16string::const_iterator& begin,
                      const std::u16string::const_iterator& end,
                      long default_value,
                      const char16_t thousands_separator,
                      int iBase
                    ) noexcept
{
    std::u16string::const_iterator mutable_begin = begin;
    return parse_long(mutable_begin, end, default_value, thousands_separator, iBase);
}


long	parse_long 	( const std::u16string& sString,
                      long default_value,
                      const char16_t thousands_separator,
                      int iBase
                    ) noexcept
{
    return parse_long(sString.begin(), sString.end(), default_value, thousands_separator, iBase);
}




bool parse_int (int& value,
                  const std::u16string::const_iterator& begin,
                  const std::u16string::const_iterator& end,
                  int default_value,
                  const char16_t thousands_separator,
                  int iBase
               ) noexcept
{
    std::string str = to_string(begin, end);
    std::string ts = to_string(thousands_separator);
    return cpaf::parse_int(value, str.begin(), str.end(), default_value, ts, iBase);
}

bool parse_int (int& value,
                  const std::u16string& sString,
                  int default_value,
                  const char16_t thousands_separator,
                  int iBase
              ) noexcept
{
    return parse_int (value, sString.begin(), sString.end(), default_value, thousands_separator, iBase);
}


int parse_int 	( std::u16string::const_iterator &begin,
                  const std::u16string::const_iterator& end,
                  int default_value,
                  const char16_t thousands_separator,
                  int iBase
                    ) noexcept
{
    int value;
    parse_int(value, begin, end, default_value, thousands_separator, iBase);
    return value;
}

int 	parse_int 	( const std::u16string::const_iterator& begin,
                      const std::u16string::const_iterator& end,
                      int default_value,
                      const char16_t thousands_separator,
                      int iBase
                    ) noexcept
{
    std::u16string::const_iterator mutable_begin = begin;
    return parse_int(mutable_begin, end, default_value, thousands_separator, iBase);
}


int	parse_int 	( const std::u16string& sString,
                      int default_value,
                      const char16_t thousands_separator,
                      int iBase
                    ) noexcept
{
    return parse_int(sString.begin(), sString.end(), default_value, thousands_separator, iBase);
}
} // END namespace cpaf::unicode

