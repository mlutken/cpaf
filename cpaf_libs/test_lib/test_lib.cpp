

#include <iostream> 
#include <string> 
#include <utf8/checked.h> 

 
void 
utf8_test()
{
	using namespace utf8::convenience;
	std::cout << "utf8_test()" << std::endl;
	std::string s = replace_invalid_copy( std::string("REPLACE_INVALID_COPY"), 32 );
	std::cout << "utf8_test(): '" << s << "'" << std::endl;
}
 
 

/** Return true if string can only represent the beginning of a path.
In case false is returned the string could still represent a path, but then it 
depends on the context (ie. within the whole URL. */
bool TEST_is_definitely_a_path( const std::string& s, std::string::size_type iCurPos  )
{
	using namespace std;
	if ( (s.size() - iCurPos  >= 2) && s[iCurPos] == '.' && s[iCurPos+1] == '.'	)	return true;	// ..
	if ( (s.size() - iCurPos  >= 2) && s[iCurPos] == '.' && s[iCurPos+1] == '/'	)	return true;	// ./
	if ( (s.size() - iCurPos  >= 1) && s[iCurPos] == '/'						)	return true;	// /
//OUT:	if ( boost::icontains( s, "D.asp") )	return true;
	string::size_type i = s.find('.', iCurPos );
	if ( i != string::npos && ( (i+3) < s.size() ) ) {
		string sTest = s.substr( i, 4 );
		//cout << "sTest: " << sTest << endl; 
		if ( sTest == ".asp" || sTest == ".php" )	return true;
	}
	return false;	
}

