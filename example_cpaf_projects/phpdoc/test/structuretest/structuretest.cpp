
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

#include "adhoc_structure_tests.h"

using namespace std;
using namespace phpdoc;
using namespace boost::algorithm;



int main(int argc, char* argv[])
{
    int retVal = 0;
//    retVal = basicTest_ExNavigatorAndSelects(argc, argv);
    retVal = navigatorTest(argc, argv);
    return retVal;
}

