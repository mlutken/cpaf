#ifndef _PHP_DOCUMENT_UTILS_H_
#define _PHP_DOCUMENT_UTILS_H_

#include "phpdoc_types.h"

namespace phpdoc {

ParseString     toParseString   ( const std::string& str);
std::string     toStdString     ( const ParseString& str);
std::string     toStdString     ( const Iterator& first, const Iterator& last);

// -----------------------
// --- Debug functions ---
// -----------------------



} // END namespace phpdoc


#endif //_PHP_DOCUMENT_UTILS_H_

