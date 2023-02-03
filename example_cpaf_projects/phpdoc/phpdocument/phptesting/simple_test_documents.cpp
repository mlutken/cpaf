#include "simple_test_documents.h"

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
#include <phpstructure/QuotedStringList.h>

#include <phpstructure/settingSet.h>
#include <phpstructure/navDropDownSelectCreate.h>
#include <phpstructure/navJavascriptExecuteCreate.h>
#include <phpstructure/navDomPosClickCreate.h>
#include <phpstructure/navInputOptionsCreate.h>
#include <phpstructure/navUrlLoadCreate.h>
#include <phpstructure/navAttributeNamesCreate.h>
#include <phpstructure/navOptionAdd.h>
#include <phpstructure/Navigator.h>
#include <phpstructure/NavWheel.h>
#include <phpstructure/DomSequenceList.h>
#include <phpstructure/DomSequenceElement.h>

using namespace std;

namespace phpdoc {

// -----------------------------
// --- Navigator test create ---
// -----------------------------

File* create_Navigator()
{
    File* f = new File();

    f->appendNode(IncludeStatement::create()->isOnceSet(true)->valueSet("webminer.php"));
    f->appendNode(IncludeStatement::create()->valueSet("Airplay1.tpl.php"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));

    f->appendNode(navJavascriptExecuteCreate::create()->actionNameSet("clothesSelect"));
    f->appendNode(navUrlLoadCreate::create()->baseUrlSet("http://examples.netscavator.com/sites/")->actionNameSet("clothesSelect"));
    f->appendNode(navDropDownSelectCreate::create()->actionNameSet("clothesSelect"));
    f->appendNode(navInputOptionsCreate::create()->actionNameSet("clothesSelect"));
    f->appendNode(navDomPosClickCreate::create()->findNextSequenceSet("clothesSelectSubSeq")->actionNameSet("clothesSelect"));


    f->appendNode(navAttributeNamesCreate::create()->valueSet("media_format_name, media_type_name"));
    f->appendNode(navOptionAdd::create()->countSet(1)->attributeValuesSet("MP3, audio"));
    f->appendNode(navOptionAdd::create()->countSet(2)->attributeValuesSet("CD, audio"));

    QuotedStringList* ql = QuotedStringList::create()->valueSet<QuotedStringList>("A, B");
    cout << "A QuotedStringList::value: " << ql->value() << " size: " << ql->size() << endl;
    ql->stringVectorSet({"dsrf", "sdf", "xcgf"});
    cout << "B QuotedStringList::value: " << ql->value() << " size: " << ql->size() << endl;
    f->appendNode(ql);
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));

    NavWheel* nw = NavWheel::create();
    f->appendNode(nw);
    nw->optionCreateSet(navDropDownSelectCreate::create()->actionNameSet("clothesSelect"));
//    nw->attributeNamesSet(navAttributeNamesCreate::create()->valueSet<navAttributeNamesCreate>("media_format_name, media_type_name"));

    nw->childEnabledSet(true, NavWheel::ATTRIBUTE_NAMES_POS);
    nw->attributeNames()->attributeNames()->stringValueSet("media_format_name", 0);
    nw->attributeNames()->attributeNames()->stringValueSet("media_type_name", 1);
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));

    DomSequenceList* dsl = DomSequenceList::create()->valueSet<DomSequenceList>("DIV, class, #eq:myClass");
    cout << "DomSequenceList::value: " << dsl->value() << " size: " << dsl->size() << endl;
    cout << "Element 2: Compare fun : " << dsl->get<DomSequenceElement>(2)->compareFunction() << endl;
    cout << "Element 2: String value: " << dsl->get<DomSequenceElement>(2)->stringValue() << endl;

    exit(0);
    return f;
}


// ------------------------------------------------
// --- ExNavigatorAndSelects (basic)test create ---
// ------------------------------------------------

File* create_ExNavigatorAndSelects()
{
    File* f = new File();
    FunctionDefinition* fn;
    StatementsBlock* sb;

    f->appendNode(IncludeStatement::create()->isOnceSet(true)->valueSet("webminer.php"));
    f->appendNode(CommentSingleLine::create()->valueSet(" ----------------------------"));
    f->appendNode(CommentSingleLine::create()->valueSet(" --- Global configuration ---"));
    f->appendNode(CommentSingleLine::create()->valueSet(" ----------------------------"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));
    f->appendNode(
                CrawlerSettingsFunctionDefinition::create()->wsBeforeStatementSet("\t")->quoteTypeSet(QuotedString::DoubleQuoteE)->oneLinerSet(true)
                ->nameSet("Crawler___outputName")
                ->valueSet("ExNavigatorAndSelects"));
    f->appendNode(
                CrawlerSettingsFunctionDefinition::create()->wsBeforeStatementSet("\t")->quoteTypeSet(QuotedString::DoubleQuoteE)->oneLinerSet(true)
                ->nameSet("Crawler__urlSite")
                ->valueSet("examples.netscavator.com/sites/ExNavigatorAndSelects/index.php"));
    f->appendNode(
                CrawlerSettingsFunctionDefinition::create()->wsBeforeStatementSet("\t")->quoteTypeSet(QuotedString::DoubleQuoteE)->oneLinerSet(true)
                ->nameSet("Crawler__urlStart")
                ->valueSet("examples.netscavator.com/sites/ExNavigatorAndSelects/dosearch.php"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));


    fn = FunctionDefinition::create();
    sb = fn->statements();
    f->appendNode(fn->nameSet("Crawler__beginMining"));

    //    settingSet		( "item_base_reliability", "0");;
    //    settingSet		( "color", "blue");;
    sb->appendNode( settingSet::create()->nameSet("item_base_reliability")->valueSet("0") );
    sb->appendNode( settingSet::create()->wsBeforeStatementSet("\t")->nameSet("color")->valueSet("blue") );

    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));

    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\t"));
    sb->appendNode(CommentSingleLine::create()->valueSet(" Define dom sequences for the positions we need"));


    //    domSeqCreate  ( "DoSearchBtnSeq"    , "INPUT, value, Search"   , "equals"   );
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("DoSearchBtnSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("INPUT, value, Search"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("equals"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("0"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("domSeqCreate")
    );

    //    domSeqCreate  ( "ProductStartSeq"   , "DIV, class, product"    , "equals"   );
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("ProductStartSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("DIV, class, product"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("equals"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("0"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("domSeqCreate")
    );

    //    domSeqCreate  ( "ProductNameSeq"    , ""                       , "equals"   ); // Empty means never find => fallback to navigator attribute
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("ProductNameSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("equals"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("0"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("domSeqCreate")
    );

    //    domSeqCreate  ( "ProductPriceSeq"   , "EUR"                    , "contains" );
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("ProductPriceSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("EUR"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("contains"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("0"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("domSeqCreate")
    );


    //    domSeqCreate  ( "DoClickNextSeq"    , "Next, /A"                , "contains", -2    );
    //    // (NOTE: we step back so we are sure to be within the A tag when we 'click' on the link.)
    //
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("DoClickNextSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("Next, /A"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("contains"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("-2"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("domSeqCreate")
    );
    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\t"));
    sb->appendNode(CommentSingleLine::create()->valueSet(" (NOTE: we step back so we are sure to be within the A tag when we 'click' on the link.)"));
    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));

    //    domSeqCreate  ( "ClothesSelectSeq"  	, "SELECT, name, clothes_type"  , "equals" 	);
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("ClothesSelectSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("SELECT, name, clothes_type"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("equals"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("0"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("domSeqCreate")
    );

    //    domSeqCreate  ( "FabricSelectSeq"   	, "SELECT, name, fabric_type"   	, "equals"     );
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("FabricSelectSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("SELECT, name, fabric_type"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("equals"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("0"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("domSeqCreate")
    );

    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));
    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));


    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\t"));
    sb->appendNode(CommentSingleLine::create()->valueSet(" Navigator"));


    //    navDropDownSelectCreate("clothesSelect");
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("clothesSelect"))->whiteSpaceBeforeSet("")
        )->wsBeforeStatementSet("\t")->nameSet("navDropDownSelectCreate")
    );

    //    navAttributeNamesCreate ("product_name, sex");
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("product_name, sex"))->whiteSpaceBeforeSet("")
        )->wsBeforeStatementSet("\t")->nameSet("navAttributeNamesCreate")
    );

    //    navOptionAdd ( 1,  "dresses"        	, ATTR, "Dress,  F"  	);
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("1"))->whiteSpaceBeforeSet("")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("dresses"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(IntegerConstant::create()->nameSet("ATTR"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("Dress,  F"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("navOptionAdd")
    );

    //    navOptionAdd ( 2,  "ties"           	, ATTR, "Tie,    M"  	);
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("2"))->whiteSpaceBeforeSet("")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("ties"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(IntegerConstant::create()->nameSet("ATTR"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("Tie,    M"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("navOptionAdd")
    );

    //    navOptionAdd ( 3,  "jackets"        	, ATTR, "Jacket, MF" 	);
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("3"))->whiteSpaceBeforeSet("")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("jackets"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(IntegerConstant::create()->nameSet("ATTR"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("Jacket, MF"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("navOptionAdd")
    );

    //    navDropDownSelectCreate("fabricSelect");
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("fabricSelect"))->whiteSpaceBeforeSet("")
        )->wsBeforeStatementSet("\t")->nameSet("navDropDownSelectCreate")
    );

    //    navAttributeNamesCreate ("fabric");
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("fabric"))->whiteSpaceBeforeSet("")
        )->wsBeforeStatementSet("\t")->nameSet("navAttributeNamesCreate")
    );

    //    navOptionAdd ( 1,  "wool"           	, ATTR, "Wool"       	);
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("1"))->whiteSpaceBeforeSet("")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("wool"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(IntegerConstant::create()->nameSet("ATTR"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("Wool"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("navOptionAdd")
    );

    //    navOptionAdd ( 2,  "cotton"         	, ATTR, "Cotton"     	);
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("2"))->whiteSpaceBeforeSet("")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("cotton"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(IntegerConstant::create()->nameSet("ATTR"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("Cotton"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("navOptionAdd")
    );


    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));
    f->appendNode(CommentSingleLine::create()->valueSet(" -------------------------------"));
    f->appendNode(CommentSingleLine::create()->valueSet(" --- DoSearch page functions ---"));
    f->appendNode(CommentSingleLine::create()->valueSet(" -------------------------------"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));


//    function ExNavigatorAndSelects__DoSearch__match()
//    {
//        return contains_fn( urlCurrent() , "dosearch.php" );  // Handle do search page.
//    }
    fn = FunctionDefinition::create();
    sb = fn->statements();
    f->appendNode(fn->nameSet("Crawler__DoSearch__match"));

    sb->appendNode(
        CodeBlop::create()->wsBeforeStatementSet("\t")->endsWithSemicolonSet(false)->valueSet("return contains_fn( urlCurrent() , \"dosearch.php\" );")
    );

    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\t"));
    sb->appendNode(CommentSingleLine::create()->valueSet(" Handle do search page."));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));


//    function Crawler__DoSearch__mine()
//    {
//        // Nothing to mine on the "do search" page
//    }
    fn = FunctionDefinition::create();
    sb = fn->statements();
    f->appendNode(fn->nameSet("Crawler__DoSearch__mine"));
    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\t"));
    sb->appendNode(CommentSingleLine::create()->valueSet(" Nothing to mine on the \"do search\" page"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));


//    function Crawler__DoSearch__navigate()
//    {
//        doActionFind ( 1, "ClothesSelectSeq",   "clothesSelect" ); // Select clothes drop down according to current navigator state
//        doActionFind ( 1, "FabricSelectSeq",    "fabricSelect"  ); // Select fabric drop down according to current navigator state
//        doActionFind ( 1, "DoSearchBtnSeq",     "click" );         // Click on search button.
//        return true;  // Return 'true' since the navigation results in loading of a new page.
//    }
    fn = FunctionDefinition::create();
    sb = fn->statements();
    f->appendNode(fn->nameSet("Crawler__DoSearch__navigate"));

    //        doActionFind ( 1, "ClothesSelectSeq",   "clothesSelect" ); // Select clothes drop down according to current navigator state
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("1"))->whiteSpaceBeforeSet("")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("ClothesSelectSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("clothesSelect"))->whiteSpaceBeforeSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("doActionFind")
    );

    //        doActionFind ( 1, "FabricSelectSeq",    "fabricSelect"  ); // Select fabric drop down according to current navigator state
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("1"))->whiteSpaceBeforeSet("")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("FabricSelectSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("fabricSelect"))->whiteSpaceBeforeSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("doActionFind")
    );

    //        doActionFind ( 1, "DoSearchBtnSeq",     "click" );         // Click on search button.
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("1"))->whiteSpaceBeforeSet("")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("DoSearchBtnSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("click"))->whiteSpaceBeforeSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("doActionFind")
    );
    sb->appendNode(
        CodeBlop::create()->wsBeforeStatementSet("\t")->endsWithSemicolonSet(false)->valueSet("\treturn true;\n")
    );
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));


    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));
    f->appendNode(CommentSingleLine::create()->valueSet(" --------------------------------"));
    f->appendNode(CommentSingleLine::create()->valueSet(" --- Search listing functions ---"));
    f->appendNode(CommentSingleLine::create()->valueSet(" --------------------------------"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));


//    function Crawler__SearchListing__match()
//    {
//        return contains_fn( urlCurrent() , "searchlisting" );  // Handle any page with 'searchlisting' in the URL.
//    }
    fn = FunctionDefinition::create();
    sb = fn->statements();
    f->appendNode(fn->nameSet("Crawler__SearchListing__match"));

    sb->appendNode(
        CodeBlop::create()->wsBeforeStatementSet("\t")->endsWithSemicolonSet(false)->valueSet("return contains_fn( urlCurrent() , \"searchlisting\" );")
    );

    sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\t"));
    sb->appendNode(CommentSingleLine::create()->valueSet(" Handle any page with 'searchlisting' in the URL."));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));


//    function Crawler__SearchListing__mine()
//    {
//        domLoopAll("ProductStartSeq", "Loop__SearchListing__GetProduct" );
//    }
    fn = FunctionDefinition::create();
    sb = fn->statements();
    f->appendNode(fn->nameSet("Crawler__SearchListing__mine"));
    sb->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("ProductStartSeq"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("Loop__SearchListing__GetProduct"))->whiteSpaceBeforeSet(" ")
        )->wsBeforeStatementSet("\t")->nameSet("domLoopAll")
    );

//    function Loop__SearchListing__GetProduct()
//    {
//       // Get product name and  product price
//        contextBegin("product");
//        outputValueFind( 1, "ProductNameSeq", "product_name", "eraseAll", ":" ); // Get value from Navigator attribute by fallback on fail to find dom pos
//        outputValueFind( 1, "ProductPriceSeq", "product_price", "trimNumber" );
//        outputValueFind( 1, "", "sex" );   // Get value from Navigator attribute by fallback on fail to find dom pos
//        outputDirect ( "fabric" , navAttributeGet("fabric") );  // Gettting the attribute value directly
//        outputDirect ( "currency_name" , "EUR" );
//        outputDirect ( "navigator_state", navStateString() );   // Write the navigator state for demo/debug
//        outputDirect ( "nav_state_indices", navCurrentStateIndicesGet() );   // Write the navigator state indices for demo/debug
//        outputDirect ( "dbg_state_progress", (string)navLastStateIndexGet() . " / " . navStateIndexGet() );
//        contextEnd();
//    }
        fn = FunctionDefinition::create();
        sb = fn->statements();
        f->appendNode(fn->nameSet("Loop__SearchListing__GetProduct"));
        sb->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\t"));
        sb->appendNode(CommentSingleLine::create()->valueSet(" Get product name and  product price"));

        //        contextBegin("product");
        sb->appendNode(
            FunctionCall::create()->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("product"))->whiteSpaceBeforeSet(" ")
            )->wsBeforeStatementSet("\t")->nameSet("contextBegin")
        );
        //        outputValueFind( 1, "ProductNameSeq", "product_name", "eraseAll", ":" ); // Get value from Navigator attribute by fallback on fail to find dom pos
        sb->appendNode(
            FunctionCall::create()->parameterAdd (
                        CallParameter::create()->parameterSet(Integer::create()->valueSet("1"))->whiteSpaceBeforeSet("")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("ProductNameSeq"))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("product_name"))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("eraseAll"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet(":"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
            )->wsBeforeStatementSet("\t")->nameSet("outputValueFind")
        );

        //        outputValueFind( 1, "ProductPriceSeq", "product_price", "trimNumber" );
        sb->appendNode(
            FunctionCall::create()->parameterAdd (
                        CallParameter::create()->parameterSet(Integer::create()->valueSet("1"))->whiteSpaceBeforeSet("")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("ProductPriceSeq"))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("product_price"))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("trimNumber"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
            )->wsBeforeStatementSet("\t")->nameSet("outputValueFind")
        );

        //        outputValueFind( 1, "", "sex" );   // Get value from Navigator attribute by fallback on fail to find dom pos
        sb->appendNode(
            FunctionCall::create()->parameterAdd (
                        CallParameter::create()->parameterSet(Integer::create()->valueSet("1"))->whiteSpaceBeforeSet("")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet(""))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("sex"))->whiteSpaceBeforeSet(" ")
            )->wsBeforeStatementSet("\t")->nameSet("outputValueFind")
        );

        //        outputDirect ( "fabric" , navAttributeGet('fabric') );  // Gettting the attribute value directly
        sb->appendNode(
            FunctionCall::create()->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("fabric"))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(CodeBlop::create()->valueSet("navAttributeGet('fabric')"))->whiteSpaceBeforeSet(" ")
            )->wsBeforeStatementSet("\t")->nameSet("outputDirect")
        );

        //        outputDirect ( "currency_name" , "EUR" );
        sb->appendNode(
            FunctionCall::create()->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("currency_name"))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("EUR"))->whiteSpaceBeforeSet(" ")
            )->wsBeforeStatementSet("\t")->nameSet("outputDirect")
        );

        //        outputDirect ( "navigator_state", navStateString() );   // Write the navigator state for demo/debug
        sb->appendNode(
            FunctionCall::create()->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("navigator_state"))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(CodeBlop::create()->valueSet("navStateString()"))->whiteSpaceBeforeSet(" ")
            )->wsBeforeStatementSet("\t")->nameSet("outputDirect")
        );

        //        outputDirect ( "nav_state_indices", navCurrentStateIndicesGet() );   // Write the navigator state indices for demo/debug
        sb->appendNode(
            FunctionCall::create()->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("nav_state_indices"))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(CodeBlop::create()->valueSet("navCurrentStateIndicesGet()"))->whiteSpaceBeforeSet(" ")
            )->wsBeforeStatementSet("\t")->nameSet("outputDirect")
        );

        //        outputDirect ( "dbg_state_progress", (string)navLastStateIndexGet() . " / " . navStateIndexGet() );
        sb->appendNode(
            FunctionCall::create()->parameterAdd (
                        CallParameter::create()->parameterSet(QuotedString::create()->valueSet("dbg_state_progress"))->whiteSpaceBeforeSet(" ")
            )->parameterAdd (
                        CallParameter::create()->parameterSet(CodeBlop::create()->valueSet("(string)navLastStateIndexGet() . ' / ' . navStateIndexGet()"))->whiteSpaceBeforeSet(" ")
            )->wsBeforeStatementSet("\t")->nameSet("outputDirect")
        );

        //        contextEnd();
        sb->appendNode(
            FunctionCall::create()->wsBeforeStatementSet("\t")->nameSet("contextEnd")
        );

//        function Crawler__SearchListing__navigate()
//        {
//            if ( domFind ( 1, "DoClickNextSeq" ) ) {
//                nodeClick();
//                return true;    // if we find a 'next' link then a new page will be loaded
//            }
//            else {
//                if ( !navNavigationDone() ) {
//                    navNextState(); // Go to next state in navigator
//                    urlLoad ( "examples.netscavator.com/sites/ExNavigatorAndSelects/dosearch.php" );  // Back to search page
//                    return true;
//                }
//                return false;   // No 'next' link, no new page will be loaded. Miner closes down.
//            }
//        }
        fn = FunctionDefinition::create();
        sb = fn->statements();
        f->appendNode(fn->nameSet("Crawler__SearchListing__navigate"));
        const char* szCode = R"RAWSTRING(
    if ( domFind ( 1, "DoClickNextSeq" ) ) {
        nodeClick();
        return true;    // if we find a 'next' link then a new page will be loaded
    }
    else {
        if ( !navNavigationDone() ) {
            navNextState(); // Go to next state in navigator
            urlLoad ( "examples.netscavator.com/sites/ExNavigatorAndSelects/dosearch.php" );  // Back to search page
            return true;
        }
        return false;   // No 'next' link, no new page will be loaded. Miner closes down.
    }
)RAWSTRING";


        sb->appendNode(
            CodeBlop::create()->wsBeforeStatementSet("\t")->endsWithSemicolonSet(false)->valueSet(szCode)
        );

    return f;
}



// -----------------------
// --- create_testdoc1 ---
// -----------------------

File* create_testdoc1()
{
    File* f = new File();

    f->appendNode(IncludeStatement::create()->isOnceSet(true)->valueSet("webminer.php"));
    f->appendNode(IncludeStatement::create()->valueSet("Airplay1.tpl.php"));
    f->appendNode(CommentSingleLine::create()->valueSet(" ----------------------------"));
    f->appendNode(CommentSingleLine::create()->valueSet(" --- Global configuration ---"));
    f->appendNode(CommentSingleLine::create()->valueSet(" ----------------------------"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));
    f->appendNode(CommentMultiLine::create()->valueSet("A multiline\n comment"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));
    //f->appendNode(FunctionDefinition::createDbg1());
    f->appendNode(CrawlerSettingsFunctionDefinition::create()->nameSet("Crawler_urlSite")->valueSet("www.gaffa.dk"));
    f->appendNode(WhiteSpaceAndComments::create()->whiteSpaceSet("\n"));
    f->appendNode(
        FunctionCall::create()->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("searchStringSelect__FIND"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("name, field-artist, class"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(QuotedString::create()->valueSet("equals"))->whiteSpaceBeforeSet(" ")
        )->parameterAdd (
                    CallParameter::create()->parameterSet(Integer::create()->valueSet("2"))->whiteSpaceBeforeSet(" ")->whiteSpaceAfterSet(" ")
        )->nameSet("domSeqCreate")
    );

    return f;
}

} // END namespace phpdoc

