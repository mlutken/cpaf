#ifndef STRUCTURE_DEBUG_H
#define STRUCTURE_DEBUG_H

#include <iostream>
#include <string>

#include "phpdoc_types.h"

namespace phpdoc {


std::string dbgStr(const Node* node);
std::string dbgStr(const NodeVecT results);

template <typename T>
void dbgPrint(const T& t)
{
    std::cout << dbgStr(t) << std::endl;
}

} // END namespace phpdoc

#endif // STRUCTURE_DEBUG_H
