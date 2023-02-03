#include <iostream>
#include <string>
#include <complex>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>


namespace cpaf {

/** 
Parse one double from  part of an input like eg a std::string.
\return True if parsing succeeded, false otherwise.
*/
template <typename Iterator>
bool parse_double_generic( 
	Iterator first, 	///< [in] Iterator to begining of 'string' to parse
	Iterator last, 		///< [in] Iterator to end of 'string' to parse
	double& fDouble 	///< [out] Result (double)
)
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    using qi::char_;
    using qi::double_;
    using qi::_1;
    using ascii::space;
    using phoenix::ref;

	bool bParsedOk = qi::phrase_parse(first, last,
		//  Begin grammar
		(
				double_	[ref(fDouble) = _1]
			>>	*char_
		),
		//  End grammar
		space);
	
	if ( !bParsedOk || first != last ) return false; // fail if we did not get a full match
		
	return bParsedOk;
}


/** 
Parse one float from  part of an input like eg a std::string.
\return True if parsing succeeded, false otherwise.
*/
template <typename Iterator>
bool parse_float_generic( 
	Iterator first, 	///< [in] Iterator to begining of 'string' to parse
	Iterator last, 		///< [in] Iterator to end of 'string' to parse
	float& fFloat 		///< [out] Result (float)
)
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    using qi::float_;
    using qi::_1;
    using ascii::space;
    using phoenix::ref;

	bool bParsedOk = qi::phrase_parse(first, last,
		//  Begin grammar
		(
			float_[ref(fFloat) = _1]
		),
		//  End grammar
		space);
	
	if ( !bParsedOk || first != last ) return false; // fail if we did not get a full match
		
	return bParsedOk;
}


/** 
Parse one int from  part of an input like eg a std::string.
\return True if parsing succeeded, false otherwise.
*/
template <typename Iterator>
bool parse_int_generic( 
	Iterator first, 	///< [in] Iterator to begining of 'string' to parse
	Iterator last, 		///< [in] Iterator to end of 'string' to parse
	int& fInt 	///< [out] Result (int)
)
{
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    using qi::int_;
    using qi::_1;
    using ascii::space;
    using phoenix::ref;

	bool bParsedOk = qi::phrase_parse(first, last,
		//  Begin grammar
		(
			int_[ref(fInt) = _1]
		),
		//  End grammar
		space);
	
	if ( !bParsedOk || first != last ) return false; // fail if we did not get a full match
		
	return bParsedOk;
}


} //END namespace cpaf

