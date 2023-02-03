#ifndef MANIP_UTILS_H
#define MANIP_UTILS_H

#include <string>
#include <phpstructure/phpdoc_types.h>

namespace phpdoc {

class Group;
class File;

enum AddNodePositionE { eAppendNode, ePrependNode };

bool findLastInsertAfter(Group* section, const NodeSearchFunT& searchFun, Node* nodeToInsert, AddNodePositionE findFailureAddPos );
void settingSetInsert(File* f, const std::string& name, const std::string& value );

} // namespace phpdoc

#endif // MANIP_UTILS_H
