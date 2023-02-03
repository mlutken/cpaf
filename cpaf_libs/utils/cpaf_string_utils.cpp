#include <algorithm>
#include <string>

#include "cpaf_string_utils.hpp"
#include "cpaf_parse_numbers_generic.hpp"

namespace cpaf
{

//https://stackoverflow.com/questions/26080829/detecting-strtol-failure

std::string simplify_whitespace ( const std::string& sInput           ///< [in] Input string to trim.
                                  )
{
    using namespace boost::algorithm;

    // First replace all '&nbsp;' with normal white spaces. Note UTF8 &nbsp; == \302\240 !!
    std::string sTrim =  replace_all_copy( sInput, std::string("\302\240"), std::string(" ") );
    replace_all( sTrim, std::string("\n"), std::string(" ") );
    boost::algorithm::trim ( sTrim );
    // Primitive (slow) algorithm to clean up spaces so that no more than one space in-a-row so to speak.
    while ( contains ( sTrim, std::string("  ") )  ) {
        replace_all( sTrim, std::string("  "), std::string(" ") );	// Replace two spaces with one space.
    }
    return sTrim;
}


void splitv	(std::vector<std::string>& resultVec, const std::string& sToSplit, const std::string& sSplitAt )
{
    boost::split( resultVec, sToSplit, boost::is_any_of(sSplitAt) );
}


std::vector<std::string> splitv( const std::string& sToSplit, const std::string& sSplitAt )
{
    StringVecT sv;
    boost::split( sv, sToSplit, boost::is_any_of(sSplitAt) );
    return sv;
}


void splitv_trim	(std::vector<std::string>& resultVec, const std::string& sToSplit, const std::string& sSplitAt )
{
    boost::split(resultVec, sToSplit, boost::is_any_of(sSplitAt) );
//    for ( StringVecT::iterator it = resultVec.begin(); it != resultVec.end(); ++it ) {
//        boost::trim (*it);
//    }
    for (auto& part : resultVec) { boost::trim (part); }
}

std::vector<std::string> splitv_trim( const std::string& sToSplit, const std::string& sSplitAt )
{
    StringVecT sv;
    splitv_trim(sv, sToSplit, sSplitAt );
    return sv;
}

void split_int (std::vector<int>& resultVec, const std::string& sToSplit, const std::string& sSplitAt, int iDefault )
{
    StringVecT v;
    splitv( v, sToSplit, sSplitAt );
    StringVecT::const_iterator 			it 		= v.begin();
    const StringVecT::const_iterator 	itEND 	= v.end();

    for (  ; it != itEND; ++it ) {
        resultVec.push_back( parse_find_int( *it, iDefault) );
    }
}

std::vector<int> split_int( const std::string& sToSplit, const std::string& sSplitAt, int iDefault )
{
    IntVecT sv;
    split_int(sv, sToSplit, sSplitAt, iDefault );
    return sv;
}


void split_float (std::vector<float>& resultVec, const std::string& sToSplit, const std::string& sSplitAt, float fDefault)
{
    StringVecT v;
    splitv( v, sToSplit, sSplitAt );
    StringVecT::const_iterator 			it 		= v.begin();
    const StringVecT::const_iterator 	itEND 	= v.end();

    for (  ; it != itEND; ++it ) {
        resultVec.push_back( parse_find_float( *it, fDefault) );
    }
}

std::vector<float> split_float( const std::string& sToSplit, const std::string& sSplitAt, float fDefault )
{
    std::vector<float> sv;
    split_float(sv, sToSplit, sSplitAt, fDefault );
    return sv;
}


void join (
        std::string& sResult
        , const StringVecT& toJoin
        , const std::string& sJoinWith
        )
{
    sResult = boost::join(toJoin, sJoinWith);
}

std::string join (
        const StringVecT& toJoin
        , const std::string& sJoinWith
        )
{
    return boost::join(toJoin, sJoinWith);
}


void join (
        std::string& sResult
        , const IntVecT& toJoin
        , const std::string& sJoinWith
        )
{
    const size_t vecSize	= toJoin.size();
    for ( size_t i = 0; i < vecSize; ++i ) {
        if ( i > 0 ) sResult.append( sJoinWith );
        sResult.append( std::to_string(toJoin[i]) );
    }
}

std::string join (
        const IntVecT& toJoin
        , const std::string& sJoinWith
        )
{
    std::string sResult;
    join ( sResult, toJoin, sJoinWith );
    return sResult;
}


void join (
        std::string& sResult
        , const FloatVecT& toJoin
        , const std::string& sJoinWith
        )
{
    const size_t vecSize	= toJoin.size();
    for ( size_t i = 0; i < vecSize; ++i ) {
        if ( i > 0 ) sResult.append( sJoinWith );
        sResult.append( std::to_string(toJoin[i]) );
    }
}

std::string join (
        const FloatVecT& toJoin
        , const std::string& sJoinWith
        )
{
    std::string sResult;
    join ( sResult, toJoin, sJoinWith );
    return sResult;
}

/** Wildcard compare function (simple globbing).
\return true if \a wild matches in \a str false otherwise.  
\example 
if ( wildcmp( "blah.jpg", "bl?h.*")) {
        //we have a match!
} else {
        //no match =(
}
\endexample
\see http://www.codeproject.com/KB/string/wildcmp.aspx
\author Written by Jack Handy - jakkhandy@hotmail.com 
*/
bool wildcmp ( 	const std::string& str, 	///< [in] String to do the matching within.
                const std::string& wild ) 	///< [in] Wildcard expression to match
{
    return wildcmp_c( str.c_str(), wild.c_str() ) != 0;
}


/** Wildcard compare function (simple globbing).
\return 1 if \a wild matches in \a str 0 otherwise.  
\example 
if ( wildcmp( "blah.jpg", "bl?h.*")) {
        //we have a match!
} else {
        //no match =(
}
\endexample
\see http://www.codeproject.com/KB/string/wildcmp.aspx
\author Written by Jack Handy - jakkhandy@hotmail.com 
*/
int wildcmp_c ( const char *str, const char *wild )
{
    const char *cp = 0, *mp = 0;

    while ( ( *str ) && ( *wild != '*' ) )
    {
        if ( ( *wild != *str ) && ( *wild != '?' ) )
        {
            return 0;
        }
        wild++;
        str++;
    }

    while ( *str )
    {
        if ( *wild == '*' )
        {
            if ( !*++wild )
            {
                return 1;
            }
            mp = wild;
            cp = str+1;
        }
        else if ( ( *wild == *str ) || ( *wild == '?' ) )
        {
            wild++;
            str++;
        }
        else
        {
            wild = mp;
            str = cp++;
        }
    }

    while ( *wild == '*' )
    {
        wild++;
    }
    return !*wild;
}


// -------------------------------
// --- Parse numbers functions ---
// -------------------------------

/** 
Parse one double from  part of an input like eg a std::string.
\return True if parsing succeeded, false otherwise.
*/
bool to_double( 
        std::string::const_iterator first, 	///< [in] Iterator to begining of 'string' to parse
        std::string::const_iterator last, 	///< [in] Iterator to end of 'string' to parse
        double& fDouble 					///< [out] Result (double)
        ) noexcept
{
    return parse_double_generic( first, last, fDouble );
}



/** 
Converts/parses a string to a double.
\return True if parsing succeeded, false otherwise.
*/
bool to_double( 
        const std::string& sDouble, 	///< [in] String to convert/parse to double
        double& fDouble 				///< [out] Result (double)
        ) noexcept
{
    return to_double ( sDouble.begin(), sDouble.end(), fDouble );
}

double to_double_default(const std::string& sDouble, double fDefault) noexcept
{
    double val;
    bool bParsedOk = to_double (sDouble.begin(), sDouble.end(), val );
    if ( !bParsedOk ) val = fDefault;
    return val;
}

/** 
Converts/parses a string to a double.
\return Converted result (double).
\throws bad_conversion
*/
double to_double( 
        const std::string& sDouble 		///< [in] String to convert/parse to double
        )
{
    double fDouble;
    bool bParsedOk = to_double ( sDouble.begin(), sDouble.end(), fDouble );
    if ( !bParsedOk ) throw bad_conversion();
    return fDouble;
}


bool parse_double (
        double& value,
        std::string::const_iterator &itFirst,
        const std::string::const_iterator& itLast,
        double default_value,
        const std::string& sDecimalPoint,
        const std::string& sThousandsSep
     ) noexcept
{
    std::string sStr(itFirst, itLast);
    boost::replace_all( sStr, sThousandsSep, "" );
    boost::replace_all( sStr, sDecimalPoint, "." );

    const char* str = sStr.c_str();
    char* stopStr;
    errno = 0;
    value = strtod(str, &stopStr);
    const bool parsed_ok = (errno == 0) && (*stopStr == '\0');
    if (parsed_ok) {
        int iLen = 	stopStr - str;
        itFirst = itFirst + iLen;
    }
    else {
        value = default_value;
    }

    return value;
}

bool parse_double ( double& value,
                    const std::string::const_iterator& itFirst,
                    const std::string::const_iterator& itLast,
                    double default_value,
                    const std::string& sDecimalPoint,
                    const std::string& sThousandsSep
                   ) noexcept
{
    std::string::const_iterator begin_mutable = itFirst;
    return parse_double (value, begin_mutable, itLast, default_value, sDecimalPoint, sThousandsSep);
}

bool parse_double ( double& value,
                    const std::string& sString,
                    double default_value,
                    const std::string& sDecimalPoint,
                    const std::string& sThousandsSep) noexcept
{
    return parse_double (value, sString.begin(), sString.end(), default_value, sDecimalPoint, sThousandsSep);
}


double parse_double 	( std::string::const_iterator &itFirst,
                          const std::string::const_iterator& itLast,
                          double default_value,
                          const std::string& sDecimalPoint,
                          const std::string& sThousandsSep) noexcept
{
    double value = 0;
    if (!parse_double (value, itFirst, itLast, default_value, sDecimalPoint, sThousandsSep)) {
        value = default_value;
    }
    return value;
}

double  parse_double 	( const std::string::const_iterator& itFirst,
                          const std::string::const_iterator& itLast,
                          double default_value,
                          const std::string& sDecimalPoint,
                          const std::string& sThousandsSep) noexcept
{
    std::string::const_iterator begin_mutable = itFirst;
    return parse_double (begin_mutable, itLast, default_value, sDecimalPoint, sThousandsSep);
}

double  parse_double 	( const std::string& sString,
                          double default_value,
                          const std::string& sDecimalPoint,
                          const std::string& sThousandsSep) noexcept
{
    return parse_double (sString.begin(), sString.end(), default_value, sDecimalPoint, sThousandsSep);
}


/** Parse thru string trying to find something that can parse as a double. 
itFirst is set to rest of string. */
bool parse_find_double(double& fDouble, 							///< [out] Result (double)
        std::string::const_iterator& itFirst,
        const std::string::const_iterator& itLast,
        double default_value,
        const std::string& sDecimalPoint,
        const std::string& sThousandsSep
        ) noexcept
{
    fDouble = 0;
    if ( itFirst == itLast ) {
        return false;
    }
    struct lconv * lc;
    lc=localeconv();
    std::string sDecimalPointSystem = lc->decimal_point;

    bool bFound = true;
    std::string sStr(itFirst, itLast);
    boost::replace_all( sStr, sThousandsSep, "" );
    boost::replace_all( sStr, sDecimalPoint, sDecimalPointSystem );

    const char* szIt 	= sStr.c_str();
    const char* szStart = sStr.c_str();
    const char* szEnd 	= sStr.c_str() + sStr.length();
    char* stopStr = NULL;

    while ( fDouble == 0 && ( szIt < szEnd ) ) {
        fDouble = strtod( szIt++, &stopStr );
    }
    if ( fDouble == 0 && (*(--szIt) != '0' ) )	bFound = false;
    int iLen = 	stopStr - szStart;
    itFirst = itFirst + iLen;
    if (!bFound) {
        fDouble = default_value;
    }
    return bFound;
}

bool parse_find_double(
        double& fDouble,
        const std::string::const_iterator& begin,
        const std::string::const_iterator& end,
        double default_value,
        const std::string& sDecimalPoint,
        const std::string& sThousandsSep) noexcept
{
    std::string::const_iterator mutable_begin = begin;
    return parse_find_double(fDouble, mutable_begin, end, default_value, sDecimalPoint, sThousandsSep );
}

/** Parse thru string trying to find something that can parse as a double. */
bool parse_find_double( 
        double& fDouble, 							///< [out] Result (double)
        const std::string sInput,
        double default_value,
        const std::string& sDecimalPoint,
        const std::string& sThousandsSep
        ) noexcept
{
    return parse_find_double( fDouble, sInput.begin(), sInput.end(), default_value, sDecimalPoint, sThousandsSep );
}

double parse_find_double( 
        const std::string sInput
        , double default_value
        , const std::string& sDecimalPoint
        , const std::string& sThousandsSep) noexcept
{
    double val;
    std::string::const_iterator it = sInput.begin();
    parse_find_double( val, it, sInput.end(), default_value, sDecimalPoint, sThousandsSep );
    return val;
}

bool parse_float (float& value,
                  std::string::const_iterator &itFirst,
                  const std::string::const_iterator& itLast,
                  float default_value,
                  const std::string& sDecimalPoint,
                  const std::string& sThousandsSep) noexcept
{
    std::string sStr(itFirst, itLast);
    boost::replace_all( sStr, sThousandsSep, "" );
    boost::replace_all( sStr, sDecimalPoint, "." );

    const char* str = sStr.c_str();
    char* stopStr;
    errno = 0;
    value = strtof(str, &stopStr);
    const bool parsed_ok = (errno == 0) && (*stopStr == '\0');
    if (parsed_ok) {
        int iLen = 	stopStr - str;
        itFirst = itFirst + iLen;
    }
    else {
        value = default_value;
    }

    return value;
}

bool parse_float (float& value,
                  const std::string::const_iterator& itFirst,
                  const std::string::const_iterator& itLast,
                  float default_value,
                  const std::string& sDecimalPoint,
                  const std::string& sThousandsSep) noexcept
{
    std::string::const_iterator begin_mutable = itFirst;
    return parse_float (value, begin_mutable, itLast, default_value, sDecimalPoint, sThousandsSep);
}

bool parse_float (float& value,
                  const std::string& sString,
                  float default_value,
                  const std::string& sDecimalPoint,
                  const std::string& sThousandsSep) noexcept
{
    return parse_float (value, sString.begin(), sString.end(), default_value, sDecimalPoint, sThousandsSep);
}

float parse_float 	( std::string::const_iterator &itFirst,
                      const std::string::const_iterator& itLast,
                      float default_value,
                      const std::string& sDecimalPoint,
                      const std::string& sThousandsSep) noexcept
{
    float value = 0;
    if (!parse_float (value, itFirst, itLast, default_value, sDecimalPoint, sThousandsSep)) {
        value = default_value;
    }
    return value;
}

float  parse_float 	( const std::string::const_iterator& itFirst,
                          const std::string::const_iterator& itLast,
                          float default_value,
                          const std::string& sDecimalPoint,
                          const std::string& sThousandsSep) noexcept
{
    std::string::const_iterator begin_mutable = itFirst;
    return parse_float (begin_mutable, itLast, default_value, sDecimalPoint, sThousandsSep);
}

float  parse_float 	( const std::string& sString,
                          float default_value,
                          const std::string& sDecimalPoint,
                          const std::string& sThousandsSep) noexcept
{
    return parse_float (sString.begin(), sString.end(), default_value, sDecimalPoint, sThousandsSep);
}


////////
/** Parse thru string trying to find something that can parse as an float. 
\return True if parsing succeeded. */
bool parse_find_float (float& fVal
        , std::string::const_iterator& itFirst
        , std::string::const_iterator itLast
        , float default_value
        , const std::string& sDecimalPoint
        , const std::string& sThousandsSep
        )
{
    double dVal;
    bool parsedOk = parse_find_double (dVal, itFirst, itLast, default_value, sDecimalPoint, sThousandsSep);
    fVal = static_cast<float>(dVal);
    return parsedOk;
}

bool parse_find_float(
        float& value,
        const std::string::const_iterator& begin,
        const std::string::const_iterator& end,
        float default_value,
        const std::string& sDecimalPoint,
        const std::string& sThousandsSep) noexcept
{
    std::string::const_iterator mutable_begin = begin;
    return parse_find_float(value, mutable_begin, end, default_value, sDecimalPoint, sThousandsSep);
}

/** Parse thru string trying to find something that can parse as an float. 
\return True if parsing succeeded. */
bool parse_find_float (float& fVal
        , const std::string sInput
        , float default_value
        , const std::string& sDecimalPoint
        , const std::string& sThousandsSep
        )
{
    return parse_find_float(fVal, sInput.begin(), sInput.end(), default_value, sDecimalPoint, sThousandsSep);
}


/** Parse thru string trying to find something that can parse as an float. 
\return Value parsed or default value if parsing fails. */
float parse_find_float(
        const std::string sInput
        , float default_value
        , const std::string& sDecimalPoint
        , const std::string& sThousandsSep
        )
{
    float val;
    parse_find_float(val, sInput, default_value, sDecimalPoint, sThousandsSep);
    return val;
}

/////////

/** 
Parse one float from  part of an input like eg a std::string.
\return True if parsing succeeded, false otherwise.
*/
bool to_float( 
        std::string::const_iterator first, 	///< [in] Iterator to begining of 'string' to parse
        std::string::const_iterator last, 	///< [in] Iterator to end of 'string' to parse
        float& fFloat 						///< [out] Result (float)
        ) noexcept
{
    return parse_float_generic( first, last, fFloat );
}


/** 
Converts/parses a string to a float.
\return True if parsing succeeded, false otherwise.
*/
bool to_float( 
        const std::string& sFloat, 	///< [in] String to convert/parse to float
        float& fFloat 				///< [out] Result (float)
        ) noexcept
{
    return to_float ( sFloat.begin(), sFloat.end(), fFloat );
}


/** 
Converts/parses a string to a float.
\return Converted result (float) or default if conversion fails.
*/
float to_float_default(
        const std::string& sFloat 	///< [in] String to convert/parse to float
        , float fDefault
        ) noexcept
{
    float val;
    bool bParsedOk = to_float ( sFloat.begin(), sFloat.end(), val );
    if ( !bParsedOk ) val = fDefault;
    return val;
}

/**
Converts/parses a string to a float.
\return Converted result (float).
\throws bad_conversion
*/
float to_float(const std::string &sFloat)
{
    float fFloat;
    bool bParsedOk = to_float ( sFloat.begin(), sFloat.end(), fFloat );
    if ( !bParsedOk ) throw bad_conversion();
    return fFloat;
}

/** 
Parse one int from  part of an input like eg a std::string.
\return True if parsing succeeded, false otherwise.
*/
bool to_int( 
        std::string::const_iterator first, 	///< [in] Iterator to begining of 'string' to parse
        std::string::const_iterator last, 	///< [in] Iterator to end of 'string' to parse
        int& fInt 							///< [out] Result (int)
        ) noexcept
{
    return parse_int_generic( first, last, fInt );
}


/** 
Converts/parses a string to a int.
\return True if parsing succeeded, false otherwise.
*/
bool to_int( 
        const std::string& sInt, 	///< [in] String to convert/parse to int
        int& fInt 					///< [out] Result (int)
        ) noexcept
{
    return to_int ( sInt.begin(), sInt.end(), fInt );
}

/** 
Converts/parses a string to a int.
\return Converted result (int) or default if conversion fails.
*/
int to_int_default(
        const std::string& sInt 		///< [in] String to convert/parse to int
        , int iDefault
        ) noexcept
{
    int val;
    bool bParsedOk = to_int ( sInt.begin(), sInt.end(), val );
    if ( !bParsedOk ) val = iDefault;
    return val;
}


/** 
Converts/parses a string to a int.
\return Converted result (int).
\throws bad_conversion
*/
int to_int (
        const std::string& sInt 		///< [in] String to convert/parse to int
        )
{
    int fInt;
    bool bParsedOk = to_int ( sInt.begin(), sInt.end(), fInt );
    if ( !bParsedOk ) throw bad_conversion();
    return fInt;
}



bool parse_long (long& value,
                 std::string::const_iterator &itFirst,
                 const std::string::const_iterator& itLast,
                 long default_value,
                 const std::string& sThousandsSep,
                 int iBase
              ) noexcept
{
    std::string sStr(itFirst, itLast);
    boost::replace_all( sStr, sThousandsSep, "" );

    const char* str = sStr.c_str();
    char* stopStr;
    errno = 0;
    value = strtol(str, &stopStr, iBase);
    const bool parsed_ok = (errno == 0) && (*stopStr == '\0');
    if (parsed_ok) {
        int iLen = 	stopStr - str;
        itFirst = itFirst + iLen;
    }
    else {
        value = default_value;
    }

    return value;
}

bool parse_long (long& value,
                  const std::string::const_iterator& itFirst,
                  const std::string::const_iterator& itLast,
                  long default_value,
                  const std::string& sThousandsSep,
                  int iBase
               ) noexcept
{
    std::string::const_iterator begin_mutable = itFirst;
    return parse_long (value, begin_mutable, itLast, default_value, sThousandsSep, iBase);
}

bool parse_long (long& value,
                  const std::string& sString,
                  long default_value,
                  const std::string& sThousandsSep,
                  int iBase
              ) noexcept
{
    return parse_long (value, sString.begin(), sString.end(), default_value, sThousandsSep, iBase);
}


long parse_long 	( std::string::const_iterator &itFirst,
                      const std::string::const_iterator& itLast,
                      long default_value,
                      const std::string& sThousandsSep,
                      int iBase
                    ) noexcept
{
    long value;
    parse_long(value, itFirst, itLast, default_value, sThousandsSep, iBase);
    return value;
}

long 	parse_long 	( const std::string::const_iterator& itFirst,
                      const std::string::const_iterator& itLast,
                      long default_value,
                      const std::string& sThousandsSep,
                      int iBase
                    ) noexcept
{
    std::string::const_iterator mutable_begin = itFirst;
    return parse_long(mutable_begin, itLast, default_value, sThousandsSep, iBase);
}


long	parse_long 	( const std::string& sString,
                      long default_value,
                      const std::string& sThousandsSep,
                      int iBase
                    ) noexcept
{
    return parse_long(sString.begin(), sString.end(), default_value, sThousandsSep, iBase);
}



long parse_long ( 	std::string::iterator &itFirst, 
                    std::string::iterator itLast,
                    const std::string& sThousandsSep,
                    int iBase
                    )
{
    std::string sStr(itFirst, itLast);
    boost::replace_all( sStr, sThousandsSep, "" );

    const char* str = sStr.c_str();
    char* stopStr;
    long iVal;
    iVal = strtol(str, &stopStr, iBase);
    int iLen = 	stopStr - str;
    itFirst = itFirst + iLen;
    return iVal;
}


long parse_long (	std::string sString,
                    const std::string& sThousandsSep,
                    int iBase
                    )
{
    std::string::iterator itBegin = sString.begin();
    std::string::iterator itEnd = sString.end();
    return parse_long( itBegin, itEnd, sThousandsSep, iBase);
}



bool parse_find_long (long& lVal,
        std::string::const_iterator &itFirst,
        const std::string::const_iterator& itLast,
        long default_value,
        const std::string& sThousandsSep,
        int iBase) noexcept
{
    lVal = 0;
    if ( itFirst == itLast ) {
        return false;
    }

    bool bFound = true;
    std::string sStr(itFirst, itLast);
    boost::replace_all( sStr, sThousandsSep, "" );

    const char* szIt    = sStr.c_str();
    const char* szStart = sStr.c_str();
    const char* szEnd   = sStr.c_str() + sStr.length();
    char* stopStr = NULL;

    while ( lVal == 0 && ( szIt < szEnd ) ) {
        lVal = strtol( szIt++, &stopStr, iBase );
    }

    if ( lVal == 0 && (*(--szIt) != '0' ) )   bFound = false;
    int iLen = stopStr - szStart;
    itFirst = itFirst + iLen;
    if (!bFound) {
        lVal = default_value;
    }
    return bFound;
}

bool parse_find_long (long& lVal,
                      const std::string::const_iterator& begin,
                      const std::string::const_iterator& end,
                      long default_value,
                      const std::string& sThousandsSep,
                      int iBase
                      ) noexcept
{
    std::string::const_iterator mutable_begin = begin;
    return parse_find_long(lVal, mutable_begin, end, default_value, sThousandsSep, iBase);
}



/** Parse thru string trying to find something that can parse as a long. */
bool parse_find_long(
        long& value, 							///< [out] Result (long)
        const std::string sInput,
        long default_value,
        const std::string& sThousandsSep,
        int iBase
        ) noexcept
{
    std::string::const_iterator it = sInput.begin();
    return parse_find_long(value, it, sInput.end(), default_value, sThousandsSep, iBase);
}

/** Parse thru string trying to find something that can parse as a long. 
\return Value parsed or default value if parsing fails. */
long parse_find_long(
        const std::string sInput
        , long default_value 							///< [in] Default value in case parsing fails
        , const std::string& sThousandsSep
        , int iBase
        ) noexcept
{
    long val;
    parse_find_long(val, sInput, default_value, sThousandsSep, iBase);
    return val;
}

bool parse_int (int& value,
                 std::string::const_iterator &itFirst,
                 const std::string::const_iterator& itLast,
                 int default_value,
                 const std::string& sThousandsSep,
                 int iBase
              ) noexcept
{
    std::string sStr(itFirst, itLast);
    boost::replace_all( sStr, sThousandsSep, "" );

    const char* str = sStr.c_str();
    char* stopStr;
    errno = 0;
    value = strtol(str, &stopStr, iBase);
    const bool parsed_ok = (errno == 0) && (*stopStr == '\0');
    if (parsed_ok) {
        int iLen = 	stopStr - str;
        itFirst = itFirst + iLen;
    }
    else {
        value = default_value;
    }

    return value;
}

bool parse_int (int& value,
                  const std::string::const_iterator& itFirst,
                  const std::string::const_iterator& itLast,
                  int default_value,
                  const std::string& sThousandsSep,
                  int iBase
               ) noexcept
{
    std::string::const_iterator begin_mutable = itFirst;
    return parse_int (value, begin_mutable, itLast, default_value, sThousandsSep, iBase);
}

bool parse_int (int& value,
                  const std::string& sString,
                  int default_value,
                  const std::string& sThousandsSep,
                  int iBase
              ) noexcept
{
    return parse_int (value, sString.begin(), sString.end(), default_value, sThousandsSep, iBase);
}


int parse_int 	( std::string::const_iterator &itFirst,
                  const std::string::const_iterator& itLast,
                  int default_value,
                  const std::string& sThousandsSep,
                  int iBase
                    ) noexcept
{
    int value;
    parse_int(value, itFirst, itLast, default_value, sThousandsSep, iBase);
    return value;
}

int 	parse_int 	( const std::string::const_iterator& itFirst,
                      const std::string::const_iterator& itLast,
                      int default_value,
                      const std::string& sThousandsSep,
                      int iBase
                    ) noexcept
{
    std::string::const_iterator mutable_begin = itFirst;
    return parse_int(mutable_begin, itLast, default_value, sThousandsSep, iBase);
}


int	parse_int 	( const std::string& sString,
                      int default_value,
                      const std::string& sThousandsSep,
                      int iBase
                    ) noexcept
{
    return parse_int(sString.begin(), sString.end(), default_value, sThousandsSep, iBase);
}

/** Parse thru string trying to find something that can parse as an int. 
\return True if parsing succeeded. */
bool parse_find_int (
        int& iVal
        , std::string::const_iterator &itFirst
        , const std::string::const_iterator& itLast
        , int default_value
        , const std::string& sThousandsSep
        , int iBase
        ) noexcept
{
    long lVal;
    bool parsedOk = parse_find_long (lVal, itFirst, itLast, default_value, sThousandsSep, iBase);
    iVal = static_cast<int>(lVal);
    return parsedOk;
}

bool parse_find_int (   int& iVal
                       , const std::string::const_iterator& itFirst
                       , const std::string::const_iterator& itLast
                       , int default_value
                       , const std::string& sThousandsSep
                       , int iBase) noexcept
{
    std::string::const_iterator mutable_begin = itFirst;
    return parse_find_int(iVal, mutable_begin, itLast, default_value, sThousandsSep, iBase);
}


/** Parse thru string trying to find something that can parse as an int. 
\return True if parsing succeeded. */
bool parse_find_int (
        int& iVal
        , const std::string& sInput
        , int default_value
        , const std::string& sThousandsSep
        , int iBase
        ) noexcept
{
    return parse_find_int(iVal, sInput.begin(), sInput.end(), default_value, sThousandsSep, iBase);
}


/** Parse thru string trying to find something that can parse as an int. 
\return Value parsed or default value if parsing fails. */
int parse_find_int(
        const std::string& sInput
        , int default_value 							///< [in] Default value in case parsing fails
        , const std::string& sThousandsSep
        , int iBase
        ) noexcept
{
    int val;
    parse_find_int(val, sInput, default_value, sThousandsSep, iBase);
    return val;
}


} // END namespace cpaf

