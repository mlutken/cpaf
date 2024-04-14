#pragma once

#include <memory>
#include <mutex>
#include <cpaf_libs/locale/translator_if.h>

namespace cpaf::locale {


/**

*/
class translator
{
public:
    translator();
    explicit translator(std::unique_ptr<translator_if> translator);

    void                    set_translator  (std::unique_ptr<translator_if> translator);


    std::string             tr              (const std::string& text) const;
    std::string             target_language () const { return translator_->target_language(); }
    std::string             id              () const { return translator_->id(); }

private:
    std::unique_ptr<translator_if>  translator_;
    mutable std::mutex              access_mutex_;

};



} //END namespace cpaf::locale




