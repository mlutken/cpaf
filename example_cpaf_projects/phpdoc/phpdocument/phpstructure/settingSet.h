#ifndef PHPDOC_CRAWLER_SETTINGSET_H
#define PHPDOC_CRAWLER_SETTINGSET_H

#include <phpstructure/FunctionCall.h>

namespace phpdoc {

class settingSet : public FunctionCall
{
    PHPDOC_NODE_HDR(settingSet,FunctionCall)
public:
    static const unsigned int SETTING_NAME_POS   = 0;
    static const unsigned int SETTING_VALUE_POS  = 1;

    settingSet();

private:
    void        do_valueSet     (const std::string& val) override;
    void        do_nameSet      (const std::string& name) override;
    std::string do_valueGet     () const override;
    std::string do_nameGet      () const override;

};

} // namespace phpdoc

#endif // PHPDOC_CRAWLER_SETTINGSET_H
