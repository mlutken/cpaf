#pragma once

#include <string>
#include <unicode/cpaf_unicode_chars.hpp>

namespace cpaf::unicode {

// -------------------------------
// --- Parse numbers functions ---
// -------------------------------
/** Exception bad_conversion. */
struct bad_conversion : public virtual std::exception
{
    char const* what() const throw()	{	return "bad_conversion"; }
};

//bool 	parse_double 	( double& value,
//                          std::string::const_iterator &begin,
//                          const std::u16string::const_iterator& end,
//                          double default_value,
//                          const char16_t decimal_point = chars16::dot,
//                          const char16_t thousands_separator = chars16::comma ) noexcept;

bool 	parse_double 	( double& value,
                          const std::u16string::const_iterator& begin,
                          const std::u16string::const_iterator& end,
                          double default_value,
                          const char16_t decimal_point = chars16::dot,
                          const char16_t thousands_separator = chars16::comma) noexcept;

bool	parse_double 	( double& value,
                          const std::u16string& sString,
                          double default_value,
                          const char16_t decimal_point = chars16::dot,
                          const char16_t thousands_separator = chars16::comma) noexcept;

//double  parse_double 	( std::string::const_iterator &begin,
//                          const std::u16string::const_iterator& end,
//                          double default_value,
//                          const char16_t decimal_point = chars16::dot,
//                          const char16_t thousands_separator = chars16::comma ) noexcept;

double  parse_double 	( const std::u16string::const_iterator& begin,
                          const std::u16string::const_iterator& end,
                          double default_value,
                          const char16_t decimal_point = chars16::dot,
                          const char16_t thousands_separator = chars16::comma) noexcept;

double  parse_double 	( const std::u16string& sString,
                          double default_value,
                          const char16_t decimal_point = chars16::dot,
                          const char16_t thousands_separator = chars16::comma) noexcept;


//bool parse_find_double(
//        double& fDouble,
//        std::string::const_iterator& begin,
//        const std::u16string::const_iterator& end,
//        double default_value,
//        const char16_t decimal_point = chars16::dot,
//        const char16_t thousands_separator = chars16::comma) noexcept;

//bool parse_find_double(
//        double& fDouble,
//        const std::u16string::const_iterator& begin,
//        const std::u16string::const_iterator& end,
//        double default_value,
//        const char16_t decimal_point = chars16::dot ,
//        const char16_t thousands_separator = chars16::comma ) noexcept;

//bool parse_find_double(
//        double& fDouble,
//        const std::u16string sInput,
//        double default_value,
//        const char16_t decimal_point = chars16::dot ,
//        const std::u16string& sThousandsSep  = "," ) noexcept;

//double parse_find_double(
//        const std::u16string sInput
//        , double default_value
//        , const char16_t decimal_point = chars16::dot
//        , const std::u16string& sThousandsSep  = ",") noexcept;


//bool 	parse_float 	( float& value,
//                          std::string::const_iterator &begin,
//                          const std::u16string::const_iterator& end,
//                          float default_value,
//                          const char16_t decimal_point = chars16::dot,
//                          const char16_t thousands_separator = chars16::comma ) noexcept;

bool 	parse_float 	( float& value,
                          const std::u16string::const_iterator& begin,
                          const std::u16string::const_iterator& end,
                          float default_value,
                          const char16_t decimal_point = chars16::dot,
                          const char16_t thousands_separator = chars16::comma) noexcept;

bool	parse_float 	( float& value,
                          const std::u16string& sString,
                          float default_value,
                          const char16_t decimal_point = chars16::dot,
                          const char16_t thousands_separator = chars16::comma) noexcept;

//float  parse_float 	( std::string::const_iterator &begin,
//                          const std::u16string::const_iterator& end,
//                          float default_value,
//                          const char16_t decimal_point = chars16::dot,
//                          const char16_t thousands_separator = chars16::comma ) noexcept;

float  parse_float 	( const std::u16string::const_iterator& begin,
                          const std::u16string::const_iterator& end,
                          float default_value,
                          const char16_t decimal_point = chars16::dot,
                          const char16_t thousands_separator = chars16::comma) noexcept;

float  parse_float 	( const std::u16string& sString,
                          float default_value,
                          const char16_t decimal_point = chars16::dot,
                          const char16_t thousands_separator = chars16::comma) noexcept;

//bool parse_find_float(
//        float& fFloat,
//        std::string::const_iterator& begin,
//        std::string::const_iterator end,
//        float default_value,
//        const char16_t decimal_point = chars16::dot ,
//        const char16_t thousands_separator = chars16::comma );

//bool parse_find_float(
//        float& value,
//        const std::u16string::const_iterator& begin,
//        const std::u16string::const_iterator& end,
//        float default_value,
//        const char16_t decimal_point = chars16::dot ,
//        const char16_t thousands_separator = chars16::comma ) noexcept;

//bool parse_find_float(
//        float& fFloat,
//        const std::u16string sInput,
//        float default_value,
//        const char16_t decimal_point = chars16::dot ,
//        const std::u16string& sThousandsSep  = "," );

//float parse_find_float(
//        const std::u16string sInput
//        , float default_value
//        , const char16_t decimal_point = chars16::dot
//        , const std::u16string& sThousandsSep  = "," );


//bool 	parse_long 	( long& value,
//                      std::string::const_iterator &begin,
//                      const std::u16string::const_iterator& end,
//                      long default_value,
//                      const char16_t thousands_separator = chars16::comma,
//                      int iBase = 10
//                    ) noexcept;

bool 	parse_long 	( long& value,
                      const std::u16string::const_iterator& begin,
                      const std::u16string::const_iterator& end,
                      long default_value,
                      const char16_t thousands_separator = chars16::comma,
                      int iBase = 10
                    ) noexcept;

bool	parse_long 	( long& value,
                      const std::u16string& sString,
                      long default_value,
                      const char16_t thousands_separator = chars16::comma,
                      int iBase = 10
                    ) noexcept;

//long 	parse_long 	( std::string::const_iterator& begin,
//                      const std::u16string::const_iterator& end,
//                      long default_value,
//                      const char16_t thousands_separator = chars16::comma,
//                      int iBase = 10
//                    ) noexcept;

long 	parse_long 	( const std::u16string::const_iterator& begin,
                      const std::u16string::const_iterator& end,
                      long default_value,
                      const char16_t thousands_separator = chars16::comma,
                      int iBase = 10
                    ) noexcept;

long	parse_long 	( const std::u16string& sString,
                      long default_value,
                      const char16_t thousands_separator = chars16::comma,
                      int iBase = 10
                    ) noexcept;



//bool    parse_find_long ( long& lVal,
//                          std::string::const_iterator& begin,
//                          const std::u16string::const_iterator& end,
//                          long default_value,
//                          const char16_t thousands_separator = chars16::comma,
//                          int iBase = 10
//                          ) noexcept;

//bool    parse_find_long ( long& value,
//                          const std::u16string::const_iterator& begin,
//                          const std::u16string::const_iterator& end,
//                          long default_value,
//                          const char16_t thousands_separator = chars16::comma,
//                          int iBase = 10
//                          ) noexcept;

//bool    parse_find_long (   long& lVal,
//                            const std::u16string sInput,
//                            long default_value,
//                            const char16_t thousands_separator = chars16::comma,
//                            int iBase = 10
//                            ) noexcept;

//long    parse_find_long (   const std::u16string sInput
//                            , long default_value
//                            , const char16_t thousands_separator = chars16::comma
//                            , int iBase = 10) noexcept;

//bool 	parse_int 	( int& value,
//                      std::string::const_iterator& begin,
//                      const std::u16string::const_iterator& end,
//                      int default_value,
//                      const char16_t thousands_separator = chars16::comma,
//                      int iBase = 10
//                    ) noexcept;

bool 	parse_int 	( int& value,
                      const std::u16string::const_iterator& begin,
                      const std::u16string::const_iterator& end,
                      int default_value,
                      const char16_t thousands_separator = chars16::comma,
                      int iBase = 10
                    ) noexcept;

bool	parse_int 	( int& value,
                      const std::u16string& sString,
                      int default_value,
                      const char16_t thousands_separator = chars16::comma,
                      int iBase = 10
                    ) noexcept;

//int 	parse_int 	( std::string::const_iterator& begin,
//                      const std::u16string::const_iterator& end,
//                      int default_value,
//                      const char16_t thousands_separator = chars16::comma,
//                      int iBase = 10
//                    ) noexcept;

int 	parse_int 	( const std::u16string::const_iterator& begin,
                      const std::u16string::const_iterator& end,
                      int default_value,
                      const char16_t thousands_separator = chars16::comma,
                      int iBase = 10
                    ) noexcept;

int	parse_int 	( const std::u16string& sString,
                      int default_value,
                      const char16_t thousands_separator = chars16::comma,
                      int iBase = 10
                    ) noexcept;


//bool    parse_find_int (   int& iVal
//                           , std::string::const_iterator& begin
//                           , const std::u16string::const_iterator& end
//                           , int default_value
//                           , const char16_t thousands_separator = chars16::comma
//                           , int iBase = 10) noexcept;

//bool    parse_find_int (   int& iVal
//                           , const std::u16string::const_iterator& begin
//                           , const std::u16string::const_iterator& end
//                           , int default_value
//                           , const char16_t thousands_separator = chars16::comma
//                           , int iBase = 10) noexcept;

//bool    parse_find_int (   int& iVal
//                           , const std::u16string& sInput
//                           , int default_value
//                           , const char16_t thousands_separator = chars16::comma
//                           , int iBase = 10) noexcept;

//int     parse_find_int ( const std::u16string& sInput
//                        , int default_value
//                        , const char16_t thousands_separator = chars16::comma
//                        , int iBase = 10) noexcept;



} // END namespace cpaf::unicode


