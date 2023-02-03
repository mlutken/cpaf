#include "structure_debug.h"

#include "Node.h"

using namespace std;

namespace phpdoc {

string dbgStr(const Node* node)
{
    string s;
    if (node) s.append(node->className() + " : " + node->name() + " => " + node->value() );
    else s.append("NULL POINTER!");
    return s;
}

string dbgStr(const NodeVecT results)
{
    string s;
    for (Node* node: results) {
        s.append(dbgStr(node) + "\n");
    }
    return s;
}



} // END namespace phpdoc

