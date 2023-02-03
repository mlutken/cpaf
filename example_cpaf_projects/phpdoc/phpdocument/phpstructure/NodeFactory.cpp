#include "NodeFactory.h"

#include "braces.h"
#include "CallParameter.h"
#include "CodeBlop.h"
#include "Comma.h"
#include "Comment.h"
#include "CommentMultiLine.h"
#include "CommentSingleLine.h"
#include "Compound.h"
#include "CrawlerSettingsFunctionDefinition.h"
#include "Definition.h"
#include "DoubleQuotedString.h"
#include "File.h"
#include "Float.h"
#include "FunctionCall.h"
#include "FunctionDefinition.h"
#include "Globals.h"
#include "Group.h"
#include "Identifier.h"
#include "IncludeStatement.h"
#include "IntegerConstant.h"
#include "Integer.h"
#include "Leaf.h"
#include "NodeFactory.h"
#include "Node.h"
#include "ParametersCallList.h"
#include "ParametersDefinitionList.h"
#include "QuotedString.h"
#include "ReturnQuotedStringStatement.h"
#include "ReturnStatement.h"
#include "Semicolon.h"
#include "SingleQuotedString.h"
#include "Space.h"
#include "Statement.h"
#include "StatementsBlock.h"
#include "String.h"
#include "Variable.h"
#include "WhiteSpaceAndComments.h"
#include "WhiteSpace.h"
#include "Word.h"

#include "settingSet.h"

namespace phpdoc {

static int DUMMY_REGISTER_COUNTER = 0;

NodeFactory::NodeFactory()
{
    registerClass("CurlyBraceLeft"  , &CurlyBraceLeft::create);
    registerClass("CurlyBraceRight", &CurlyBraceRight::create);
    registerClass("ParLeft", &ParLeft::create);
    registerClass("ParRight", &ParRight::create);
    registerClass("BracketLeft", &BracketLeft::create);
    registerClass("BracketRight", &BracketRight::create);
    registerClass("CallParameter", &CallParameter::create);
    registerClass("CodeBlop", &CodeBlop::create);
    registerClass("Comma", &Comma::create);
    registerClass("Comment", &Comment::create);
    registerClass("CommentMultiLine", &CommentMultiLine::create);
    registerClass("CommentSingleLine", &CommentSingleLine::create);
    registerClass("Compound", &Compound::create);
    registerClass("CrawlerSettingsFunctionDefinition", &CrawlerSettingsFunctionDefinition::create);
    registerClass("Definition", &Definition::create);
    registerClass("File", &File::create);
    registerClass("Float", &Float::create);
    registerClass("FunctionCall", &FunctionCall::create);
    registerClass("FunctionDefinition", &FunctionDefinition::create);
    registerClass("Group", &Group::create);
    registerClass("Identifier", &Identifier::create);
    registerClass("IncludeStatement", &IncludeStatement::create);
    registerClass("IntegerConstant", &IntegerConstant::create);
    registerClass("Integer", &Integer::create);
    registerClass("Leaf", &Leaf::create);
    //registerClass("Node", &Node::create);
    registerClass("ParametersCallList", &ParametersCallList::create);
    registerClass("ParametersDefinitionList", &ParametersDefinitionList::create);
    registerClass("QuotedString", &QuotedString::create);
    registerClass("ReturnQuotedStringStatement", &ReturnQuotedStringStatement::create);
    registerClass("ReturnStatement", &ReturnStatement::create);
    registerClass("Semicolon", &Semicolon::create);
    registerClass("Empty", &Empty::create);
    registerClass("Space", &Space::create);
    registerClass("Tab", &Tab::create);
    registerClass("NewLine", &NewLine::create);
    registerClass("Statement", &Statement::create);
    registerClass("StatementsBlock", &StatementsBlock::create);
    registerClass("String", &String::create);
    registerClass("Variable", &Variable::create);
    registerClass("WhiteSpaceAndComments", &WhiteSpaceAndComments::create);
    registerClass("WhiteSpace", &WhiteSpace::create);
    registerClass("Word", &Word::create);

    // Crawler classes
    registerClass("settingSet", &settingSet::create);
}

void NodeFactory::registerClass(const std::string& className, const NodeFactory::NodeCreatorFunT& creatorFun)
{
    m_nodeCreateMap[className] = creatorFun;
}

Node* NodeFactory::create(const std::string& className) const
{
    const auto it = m_nodeCreateMap.find(className);
    if ( it == m_nodeCreateMap.end() ) return 0;
    return (it->second)();
}

} // namespace phpdoc
