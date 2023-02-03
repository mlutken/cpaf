#ifndef PHPDOC_TYPES_H
#define PHPDOC_TYPES_H

#include <string>
#include <vector>
#include <bitset>
#include <functional>
#include <boost/function.hpp>

namespace phpdoc {

class Node;

typedef std::vector<Node*>          NodeVecT;
typedef std::vector<std::string>	StringVecT;

typedef boost::function< bool ( std::string const&, std::string const& ) >	StringCompareFunT;
typedef std::function< bool ( const Node* ) >                               NodeSearchFunT;

typedef char                    ParseChar;
typedef std::string             ParseString;
typedef ParseString::iterator   Iterator;

namespace doctypes {

typedef std::bitset<128>  Type;


const Type None                                 = Type();
const Type All                                  = ~Type();

const Type CurlyBraceLeft                       = (std::uint64_t)1 << 0;
const Type CurlyBraceRight                      = (std::uint64_t)1 << 1;
const Type ParLeft                              = (std::uint64_t)1 << 2;
const Type ParRight                             = (std::uint64_t)1 << 3;
const Type BracketLeft                          = (std::uint64_t)1 << 4;
const Type BracketRight                         = (std::uint64_t)1 << 5;
const Type CallParameter                        = (std::uint64_t)1 << 6;
const Type CodeBlop                             = (std::uint64_t)1 << 7;
const Type Comma                                = (std::uint64_t)1 << 8;
const Type Comment                              = (std::uint64_t)1 << 9;
const Type CommentMultiLine                     = (std::uint64_t)1 << 10;
const Type CommentSingleLine                    = (std::uint64_t)1 << 11;
const Type Compound                             = (std::uint64_t)1 << 12;
const Type CrawlerSettingsFunctionDefinition    = (std::uint64_t)1 << 13;
const Type Definition                           = (std::uint64_t)1 << 14;
const Type File                                 = (std::uint64_t)1 << 15;
const Type Float                                = (std::uint64_t)1 << 16;
const Type FunctionCall                         = (std::uint64_t)1 << 17;
const Type FunctionDefinition                   = (std::uint64_t)1 << 18;
const Type Group                                = (std::uint64_t)1 << 19;
const Type Identifier                           = (std::uint64_t)1 << 20;
const Type IncludeStatement                     = (std::uint64_t)1 << 21;
const Type IntegerConstant                      = (std::uint64_t)1 << 22;
const Type Integer                              = (std::uint64_t)1 << 23;
const Type Leaf                                 = (std::uint64_t)1 << 24;
const Type Node                                 = (std::uint64_t)1 << 25;
const Type ParametersCallList                   = (std::uint64_t)1 << 26;
const Type ParametersDefinitionList             = (std::uint64_t)1 << 27;
const Type QuotedString                         = (std::uint64_t)1 << 28;
const Type QuotedStringBase                     = (std::uint64_t)1 << 29;
const Type QuotedStringList                     = (std::uint64_t)1 << 30;
const Type ReturnQuotedStringStatement          = (std::uint64_t)1 << 31;
const Type ReturnStatement                      = (std::uint64_t)1 << 32;
const Type Semicolon                            = (std::uint64_t)1 << 33;
const Type Empty                                = (std::uint64_t)1 << 34;
const Type Space                                = (std::uint64_t)1 << 35;
const Type Tab                                  = (std::uint64_t)1 << 36;
const Type NewLine                              = (std::uint64_t)1 << 37;
const Type Statement                            = (std::uint64_t)1 << 38;
const Type StatementsBlock                      = (std::uint64_t)1 << 39;
const Type String                               = (std::uint64_t)1 << 40;
const Type StringListElement                    = (std::uint64_t)1 << 41;
const Type Variable                             = (std::uint64_t)1 << 42;
const Type WhiteSpaceAndComments                = (std::uint64_t)1 << 43;
const Type WhiteSpace                           = (std::uint64_t)1 << 44;
const Type Word                                 = (std::uint64_t)1 << 45;
const Type DomSequenceElement                   = (std::uint64_t)1 << 46;
const Type DomSequenceList                      = (std::uint64_t)1 << 47;



const Type NavWheelCreate                       = Type().set(127);  // TODO: Out!

const Type settingSet                           = Type().set(60);
const Type navJavascriptExecuteCreate           = Type().set(61);
const Type navUrlLoadCreate                     = Type().set(62);
const Type navInputOptionsCreate                = Type().set(63);
const Type navDropDownSelectCreate              = Type().set(64);
const Type navDomPosClickCreate                 = Type().set(65);
const Type navAutoInputCreate                   = Type().set(66);
const Type navAttributeNamesCreate              = Type().set(67);
const Type navOptionAdd                         = Type().set(68);


const Type Navigator                            = Type().set(80);
const Type NavWheel                             = Type().set(81);
const Type NavOptionCreate                      = Type().set(82);
const Type NavWheelJavascriptExecute            = Type().set(83);
const Type NavWheelUrlLoad                      = Type().set(84);
const Type NavWheelInputOptions                 = Type().set(85);
const Type NavWheelDropDownSelect               = Type().set(86);
const Type NavWheelDomPosClick                  = Type().set(87);


const Type DbgTester = Type().set(56);

} // END namespace docfamily


} // END namespace phpdoc

#endif // PHPDOC_TYPES_H
