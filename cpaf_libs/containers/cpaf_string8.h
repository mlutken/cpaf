#ifndef CPAF_DYNAMIC_STRING8_H
#define CPAF_DYNAMIC_STRING8_H

#include <ostream>
#include <containers/cpaf_dynamic_array.hpp>

namespace cpaf::containers
{

using base_string8 = dynamic_array<char, 1>;

class string8 : public base_string8
{
public:
    string8()
        : base_string8()
    {
        reserve(16);
        (*this)[0] = '\0';
    }

    explicit string8(dynamic_array::size_type count)
        : base_string8(count)
    {
        (*this)[0] = '\0';
    }

    string8(const char* sz_str)
    {
        append(sz_str, std::strlen(sz_str));
    }

    string8(const std::string& str)
    {
        append(str.c_str(), str.size());
    }

    string8(std::string&& str)
    {
        append(str.c_str(), str.size());
    }

    string8& operator= (const char* sz_str)
    {
        append(sz_str, std::strlen(sz_str));
        return *this;
    }

    string8& operator= (const std::string& str)
    {
        append(str.c_str(), str.size());
        return *this;
    }

    string8& operator= (std::string& str)
    {
        append(str.c_str(), str.size());
        return *this;
    }

    string8& operator= (std::string&& str)
    {
        append(str.c_str(), str.size());
        return *this;
    }
    const char* c_str() const { return data(); }

//    operator const char*() const { return data(); }

    string8& operator+= (char c)
    {
        append(c);
        return *this;
    }

    string8& operator+= (const string8& other)
    {
        append(other);
        return *this;
    }

    string8& operator+= (const char* sz_str)
    {
        append(sz_str, std::strlen(sz_str));
        return *this;
    }

    string8& operator+= (const std::string& str)
    {
        append(str.c_str(), str.size());
        return *this;
    }

    // ----------

    const string8 operator+ (char c) const
    {
        string8 result = *this;
        result += c;
        return result;
    }

    const string8 operator+ (const string8& other) const
    {
        string8 result = *this;     // Make a copy of myself.  Same as string8 result(*this);
        result += other;            // Use += to add other to the copy.
        return result;              // All done!
    }

    const string8 operator+ (const char* sz_str) const
    {
        string8 result = *this;
        result += sz_str;
        return result;
    }

    const string8 operator+ (const std::string& str) const
    {
        string8 result = *this;
        result += str;
        return result;
    }

    friend inline std::ostream& operator<< (std::ostream& os, const string8& str);

};

} //end namespace cpaf::containers


#endif //CPAF_DYNAMIC_STRING8_H

