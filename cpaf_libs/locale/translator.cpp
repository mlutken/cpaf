
#include "translator.h"
#include "ui_translator.h"


namespace cpaf::locale {

translator::translator()
    : translator_(std::make_unique<ui_translator>())
{

}

translator::translator(std::unique_ptr<translator_if> translator)
    : translator_(std::move(translator))
{
}

void translator::set_translator(std::unique_ptr<translator_if> translator)
{
    std::scoped_lock<std::mutex> access_lock(access_mutex_);
    translator_ = std::move(translator);
}

std::string translator::tr(const std::string& text) const
{
    std::scoped_lock<std::mutex> access_lock(access_mutex_);
    return translator_->tr(text);
}




} //END namespace cpaf::locale
