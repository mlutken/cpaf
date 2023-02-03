#include "adhoc_structure_tests.h"

#include <iostream>
#include <memory>
#include <boost/algorithm/string.hpp>

#include <phpstructure/Globals.h>
#include <phpstructure/CodeBlop.h>
#include <phpstructure/CommentMultiLine.h>
#include <phpstructure/CommentSingleLine.h>
#include <phpstructure/CrawlerSettingsFunctionDefinition.h>
#include <phpstructure/Group.h>
#include <phpstructure/Leaf.h>
#include <phpstructure/Node.h>
#include <phpstructure/Space.h>
#include <phpstructure/WhiteSpace.h>
#include <phpstructure/WhiteSpaceAndComments.h>
#include <phpstructure/FunctionCall.h>
#include <phpstructure/CallParameter.h>
#include <phpstructure/Integer.h>
#include <phpstructure/IntegerConstant.h>
#include <phpstructure/Float.h>
#include <phpstructure/QuotedString.h>
#include <phpstructure/String.h>
#include <phpstructure/IntegerConstant.h>
#include <phpstructure/Identifier.h>
#include <phpstructure/Variable.h>
#include <phpstructure/IncludeStatement.h>
#include <phpstructure/StatementsBlock.h>
#include <phpstructure/structure_debug.h>
#include <phpstructure/settingSet.h>
#include <phpstructure/StringListElement.h>
#include <phpstructure/QuotedStringList.h>

#include <phptesting/simple_test_documents.h>
#include <phpmanip/search_functors.h>
#include <phpmanip/SettingsManip.h>
#include <phpmanip/manip_utils.h>


using namespace std;
using namespace phpdoc;
using namespace boost::algorithm;


int navigatorTest(int argc, char* argv[])
{
    File* crawlerFile = create_Navigator();

    cout << "Navigator manipulate test! Node class name: '" << crawlerFile->className() << "'" << endl;

    cout << "docString:\n" << crawlerFile->docString() << endl;

    return 0;
}


int basicTest_ExNavigatorAndSelects(int argc, char* argv[])
{
//    std::unique_ptr<File> n1(create_ExNavigatorAndSelects());
    File* crawlerFile = create_ExNavigatorAndSelects();

    settingSetInsert(crawlerFile, "my_setting", "my_value");
    cout << "Structure Test! Node class name: '" << crawlerFile->className() << "'" << endl;

    cout << "docString:\n" << crawlerFile->docString() << endl;
//    cout << "docString:\n" << n1->value() << endl;

//    auto func = [] (const Node* node) -> bool { return boost::algorithm::contains(node->value(), "Crawler__urlSite"); };
//    n1->search( results, func);

//    NodeVecT results;
//    crawlerFile->search( results, search::DocTypes(doctypes::settingSet));
//    dbgPrint(results);



    SettingsManip sm(crawlerFile);

    auto sf = crawlerFile->findSingle<CrawlerSettingsFunctionDefinition>( search::NameEquals("Crawler__urlSite", doctypes::CrawlerSettingsFunctionDefinition));
    if (sf) {
        sf->valueSet("examples.netscavator.com");
        //cout << "docString:\n" << crawlerFile->docString() << endl;
        //dbgPrint(sf);
    }

    auto ss = crawlerFile->findSingle<FunctionCall>( search::NameEquals("settingSet", doctypes::FunctionCall));
    if (ss) {
//        ss->valueSet("examples.netscavator.com");
        //cout << "docString:\n" << crawlerFile->docString() << endl;
//        dbgPrint(ss);
    }

//    dbgPrint(fn);

    cout << "Factory size: " << g().fac().size() << endl;
//    cout << "Node::classTypes: " << Node().classTypes() << endl;
//    cout << "FunctionCall::classTypes: " << (FunctionCall().docTypes()) << endl;
//    cout << "All types: " << doctypes::All << endl;


    StringListElement* sle = new StringListElement;
    sle->valueSet("\n\thest\n");
    dbgPrint(sle);
    cout << "StringListElement::docString():\n" << sle->docString() << endl;


    QuotedStringList* qsl = new QuotedStringList();
    qsl->quoteTypeSet(QuotedStringBase::DoubleQuoteE)->valueSet(" hej, med, dig");
    dbgPrint(qsl);
    cout << "QuotedStringList::docString():\n" << qsl->docString() << endl;

    delete crawlerFile;
    return 0;
}



