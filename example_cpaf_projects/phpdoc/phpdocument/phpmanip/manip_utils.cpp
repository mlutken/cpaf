#include "manip_utils.h"

#include <phpstructure/Group.h>
#include <phpstructure/FunctionDefinition.h>
#include <phpstructure/StatementsBlock.h>
#include <phpstructure/File.h>
#include <phpstructure/settingSet.h>
#include <phpmanip/search_functors.h>

namespace phpdoc {

bool findLastInsertAfter(Group* section, const NodeSearchFunT& searchFun, Node* nodeToInsert, AddNodePositionE findFailureAddPos)
{
    if (!section) return false;
    Node* positionNode = section->findLast<Node>(searchFun);

    bool inserted = false;
    if (positionNode) {
        Group* parent = positionNode->parent<Group>();
        if (parent) {
            parent->insertAfter(nodeToInsert, positionNode );
            inserted = true;
        }
    }

    if ((section->docTypes() & FunctionDefinition().docType()).any()) {
        section = static_cast<FunctionDefinition*>(section)->statements();
    }

    if (!inserted){
        if (eAppendNode == findFailureAddPos)   section->appendNode(nodeToInsert);
        else                                    section->prependNode(nodeToInsert);
    }

    return true;
}

void settingSetInsert(File* f, const std::string& name, const std::string& value)
{
    FunctionDefinition* beginMining = f->findSingle<FunctionDefinition>( search::NameEquals("Crawler__beginMining", doctypes::FunctionDefinition));
    if (!beginMining) beginMining = f->findSingle<FunctionDefinition>( search::NameEquals("TEMPLATE__beginMining", doctypes::FunctionDefinition));

    if (!beginMining) return;

    Node* ss = settingSet::create()->nameSet(name)->valueSet(value);
    findLastInsertAfter(beginMining, search::DocTypes(doctypes::settingSet), ss, ePrependNode );
}


} // namespace phpdoc
