#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>


namespace cpaf {
//typedef std::vector<std::string>	string_vec_t;
//typedef std::vector<int>            int_vec_t;
//typedef std::vector<float>          float_vec_t;

enum struct do_trim { no, yes };

using   StringVecT  = std::vector<std::string>;
using   IntVecT     = std::vector<int>;
using   FloatVecT   = std::vector<float>;

using str_split_const_iterator  =  boost::algorithm::split_iterator<std::string::const_iterator>;
using  str_const_range          = boost::iterator_range<std::string::const_iterator>;

template <class STRING_TYPE>
str_split_const_iterator mk_str_split_const_iterator(const STRING_TYPE& to_split, const std::string& separator)
{
    return boost::algorithm::make_split_iterator (to_split, boost::first_finder(separator, boost::algorithm::is_equal() ));
}

/** Simplify whitepace (spce, tab, newline). Removes all newlines, tabs and multiple spaces. All these are replaced by a single space. 
Any whitespace at the end or the beginning are removed.*/
std::string simplify_whitespace ( const std::string& sInput );

void                        splitv		( std::vector<std::string>& resultVec, const std::string& sToSplit, const std::string& sSplitAt= " " );
std::vector<std::string>	splitv		( const std::string& sToSplit, const std::string& sSplitAt= " " );
void                        splitv_trim	( std::vector<std::string>& resultVec, const std::string& sToSplit, const std::string& sSplitAt= " " );
std::vector<std::string>	splitv_trim	( const std::string& sToSplit, const std::string& sSplitAt= " " );

void                        split_int	( std::vector<int>& resultVec, const std::string& sToSplit, const std::string& sSplitAt= " ", int iDefault = 0 );
std::vector<int>            split_int	( const std::string& sToSplit, const std::string& sSplitAt= " ", int iDefault = 0 );
void                        split_float	( std::vector<float>& resultVec, const std::string& sToSplit, const std::string& sSplitAt= " ", float fDefault = 0 );
std::vector<float>          split_float	( const std::string& sToSplit, const std::string& sSplitAt= " ", float fDefault = 0 );

void		join		( std::string& sResult, const StringVecT& toJoin, const std::string& sJoinWith= " " );
std::string	join		( const StringVecT& toJoin, const std::string& sJoinWith= " " );

void		join		( std::string& sResult, const IntVecT& toJoin, const std::string& sJoinWith= " " );
std::string	join		( const IntVecT& toJoin, const std::string& sJoinWith= " " );

void		join		( std::string& sResult, const FloatVecT& toJoin, const std::string& sJoinWith= " " );
std::string	join		( const FloatVecT& toJoin, const std::string& sJoinWith= " " );


bool 		wildcmp 	( const std::string& str, const std::string& wild );
int 		wildcmp_c 	( const char *str, const char *wild );




// -------------------------------
// --- Parse numbers functions ---
// -------------------------------
/** Exception bad_conversion. */
struct bad_conversion : public virtual std::exception
{
    char const* what() const throw()	{	return "bad_conversion"; }
};

bool 	to_double		(std::string::const_iterator first, std::string::const_iterator last, double& fDouble) noexcept;
bool 	to_double		(const std::string& sDouble, double& fDouble) noexcept;
double 	to_double_default(const std::string& sDouble, double fDefault) noexcept;
double 	to_double		 (const std::string& sDouble);

bool 	parse_double 	( double& value,
                          std::string::const_iterator &itFirst,
                          const std::string::const_iterator& itLast,
                          double default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = "," ) noexcept;

bool 	parse_double 	( double& value,
                          const std::string::const_iterator& itFirst,
                          const std::string::const_iterator& itLast,
                          double default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = ",") noexcept;

bool	parse_double 	( double& value,
                          const std::string& sString,
                          double default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = ",") noexcept;

double  parse_double 	( std::string::const_iterator &itFirst,
                          const std::string::const_iterator& itLast,
                          double default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = "," ) noexcept;

double  parse_double 	( const std::string::const_iterator& itFirst,
                          const std::string::const_iterator& itLast,
                          double default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = ",") noexcept;

double  parse_double 	( const std::string& sString,
                          double default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = ",") noexcept;


bool parse_find_double(
        double& fDouble,
        std::string::const_iterator& itFirst,
        const std::string::const_iterator& itLast,
        double default_value,
        const std::string& sDecimalPoint = ".",
        const std::string& sThousandsSep = ",") noexcept;

bool parse_find_double(
        double& fDouble,
        const std::string::const_iterator& begin,
        const std::string::const_iterator& end,
        double default_value,
        const std::string& sDecimalPoint = "." ,
        const std::string& sThousandsSep = "," ) noexcept;

bool parse_find_double(
        double& fDouble,
        const std::string sInput,
        double default_value,
        const std::string& sDecimalPoint = "." ,
        const std::string& sThousandsSep  = "," ) noexcept;

double parse_find_double(
        const std::string sInput
        , double default_value
        , const std::string& sDecimalPoint = "."
        , const std::string& sThousandsSep  = ",") noexcept;


bool 	parse_float 	( float& value,
                          std::string::const_iterator &itFirst,
                          const std::string::const_iterator& itLast,
                          float default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = "," ) noexcept;

bool 	parse_float 	( float& value,
                          const std::string::const_iterator& itFirst,
                          const std::string::const_iterator& itLast,
                          float default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = ",") noexcept;

bool	parse_float 	( float& value,
                          const std::string& sString,
                          float default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = ",") noexcept;

float  parse_float 	( std::string::const_iterator &itFirst,
                          const std::string::const_iterator& itLast,
                          float default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = "," ) noexcept;

float  parse_float 	( const std::string::const_iterator& itFirst,
                          const std::string::const_iterator& itLast,
                          float default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = ",") noexcept;

float  parse_float 	( const std::string& sString,
                          float default_value,
                          const std::string& sDecimalPoint = ".",
                          const std::string& sThousandsSep = ",") noexcept;

bool parse_find_float(
        float& fFloat,
        std::string::const_iterator& itFirst,
        std::string::const_iterator itLast,
        float default_value,
        const std::string& sDecimalPoint = "." ,
        const std::string& sThousandsSep = "," );

bool parse_find_float(
        float& value,
        const std::string::const_iterator& begin,
        const std::string::const_iterator& end,
        float default_value,
        const std::string& sDecimalPoint = "." ,
        const std::string& sThousandsSep = "," ) noexcept;

bool parse_find_float(
        float& fFloat,
        const std::string sInput,
        float default_value,
        const std::string& sDecimalPoint = "." ,
        const std::string& sThousandsSep  = "," );

float parse_find_float(
        const std::string sInput
        , float default_value
        , const std::string& sDecimalPoint = "."
        , const std::string& sThousandsSep  = "," );


bool 	to_float		(std::string::const_iterator first, std::string::const_iterator last, float& fFloat) noexcept;
bool 	to_float		(const std::string& sFloat, float& fFloat) noexcept;
float 	to_float_default(const std::string& sFloat, float fDefault) noexcept;
float 	to_float		(const std::string& sFloat);

bool 	to_int			( std::string::const_iterator first, std::string::const_iterator last, int& fInt) noexcept;
bool 	to_int			( const std::string& sInt, int& fInt) noexcept;
int 	to_int_default	( const std::string& sInt, int iDefault) noexcept;
int 	to_int			( const std::string& sInt);


bool 	parse_long 	( long& value,
                      std::string::const_iterator &itFirst,
                      const std::string::const_iterator& itLast,
                      long default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

bool 	parse_long 	( long& value,
                      const std::string::const_iterator& itFirst,
                      const std::string::const_iterator& itLast,
                      long default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

bool	parse_long 	( long& value,
                      const std::string& sString,
                      long default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

long 	parse_long 	( std::string::const_iterator &itFirst,
                      const std::string::const_iterator& itLast,
                      long default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

long 	parse_long 	( const std::string::const_iterator& itFirst,
                      const std::string::const_iterator& itLast,
                      long default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

long	parse_long 	( const std::string& sString,
                      long default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;


long 	parse_long 		( std::string::iterator &itFirst,
                          std::string::iterator itLast,
                          const std::string& sThousandsSep = ",",
                          int iBase = 10
                          );

long 	parse_long 		(std::string sString,
                          const std::string& sThousandsSep = "," ,
                          int iBase = 10
                          );

bool    parse_find_long ( long& lVal,
                          std::string::const_iterator& itFirst,
                          const std::string::const_iterator& itLast,
                          long default_value,
                          const std::string& sThousandsSep = ",",
                          int iBase = 10
                          ) noexcept;

bool    parse_find_long ( long& value,
                          const std::string::const_iterator& begin,
                          const std::string::const_iterator& end,
                          long default_value,
                          const std::string& sThousandsSep = ",",
                          int iBase = 10
                          ) noexcept;

bool    parse_find_long (   long& lVal,
                            const std::string sInput,
                            long default_value,
                            const std::string& sThousandsSep = ",",
                            int iBase = 10
                            ) noexcept;

long    parse_find_long (   const std::string sInput
                            , long default_value
                            , const std::string& sThousandsSep = ","
                            , int iBase = 10) noexcept;

bool 	parse_int 	( int& value,
                      std::string::const_iterator &itFirst,
                      const std::string::const_iterator& itLast,
                      int default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

bool 	parse_int 	( int& value,
                      const std::string::const_iterator& itFirst,
                      const std::string::const_iterator& itLast,
                      int default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

bool	parse_int 	( int& value,
                      const std::string& sString,
                      int default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

int 	parse_int 	( std::string::const_iterator &itFirst,
                      const std::string::const_iterator& itLast,
                      int default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

int 	parse_int 	( const std::string::const_iterator& itFirst,
                      const std::string::const_iterator& itLast,
                      int default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

int	parse_int 	( const std::string& sString,
                      int default_value,
                      const std::string& sThousandsSep = ",",
                      int iBase = 10
                    ) noexcept;

bool    parse_find_int (   int& iVal
                           , std::string::const_iterator& itFirst
                           , const std::string::const_iterator& itLast
                           , int default_value
                           , const std::string& sThousandsSep = ","
                           , int iBase = 10) noexcept;

bool    parse_find_int (   int& iVal
                           , const std::string::const_iterator& itFirst
                           , const std::string::const_iterator& itLast
                           , int default_value
                           , const std::string& sThousandsSep = ","
                           , int iBase = 10) noexcept;

bool    parse_find_int (   int& iVal
                           , const std::string& sInput
                           , int default_value
                           , const std::string& sThousandsSep = ","
                           , int iBase = 10) noexcept;

int     parse_find_int ( const std::string& sInput
                        , int default_value
                        , const std::string& sThousandsSep = ","
                        , int iBase = 10) noexcept;

// ---------------------------------------
// --- Containers to strings functions ---
// ---------------------------------------

template <typename CONTAINER, typename CAST_ELEM_TO>
std::string to_hex_string(const CONTAINER& container)
{
    using boost::format;
    std::string s;
    typename CONTAINER::const_iterator it;
    for ( it = container.begin(); it != container.end(); ++it ) {
        s.append( (boost::format("%02x") % static_cast<CAST_ELEM_TO>((*it)).str()) );
    }
    return s;
}

template <typename CONTAINER>
std::string to_hex_string(const CONTAINER& container)
{
    using boost::format;
    std::string s;
    typename CONTAINER::const_iterator it;
    for ( it = container.begin(); it != container.end(); ++it ) {
        s.append( (boost::format("%02x") % (int)(*it)).str() );
    }
    return s;
}

template <typename CONTAINER, typename CAST_ELEM_TO>
std::string to_string(const CONTAINER& container, std::string fmt)
{
    using boost::format;
    std::string s;
    typename CONTAINER::const_iterator it;
    for ( it = container.begin(); it != container.end(); ++it ) {
        s.append( (boost::format(fmt) % (CAST_ELEM_TO)(*it)).str() );
    }
    return s;
}

// template <typename CONTAINER>
// std::string to_string(const CONTAINER& container, std::string fmt)
// {
//     using boost::format;
//     std::string s;
//     typename CONTAINER::const_iterator it;
//     for ( it = container.begin(); it != container.end(); ++it ) {
//         s.append( (boost::format(fmt) % (int)(*it)).str() );
//     }
//     return s;
// }

template <typename CONTAINER>
std::string container_to_string(const CONTAINER& container )
{
    std::ostringstream os;
    typename CONTAINER::const_iterator it;
    for ( it = container.begin(); it != container.end(); ++it ) {
        os << *it;
    }
    return os.str();
}


// template <class T>
// std::string to_string ( T object )
// {
//     std::ostringstream os;
//     os << object;
//     return(os.str());
// }

template <class T>
std::string to_string_c_locale ( T object )
{
    std::ostringstream os;
    os.imbue(std::locale::classic());
    os << object;
    return(os.str());
}

} // END namespace cpaf
