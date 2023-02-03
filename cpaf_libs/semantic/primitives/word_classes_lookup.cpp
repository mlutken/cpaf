#include <iostream>
#include <unicode/cpaf_u16string_utils.h>
#include <unicode/cpaf_unicode_chars.hpp>
#include "word_classes_lookup.h"

using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;

namespace cpaf::semantic
{

bool word_classes_lookup::is_class_name(const string_view& str)
{
    return !str.empty() && str[0] == chars16::left_curly_bracket;
}

void word_classes_lookup::add(const std::u16string& class_name, const std::u16string& word)
{
    by_class_map_[class_name].push_back(word);
    by_word_map_[word].push_back(class_name);
}

void word_classes_lookup::add(const u16string& class_name, std::initializer_list<u16string> words)
{
    auto& vec = by_class_map_[class_name];
    vec.insert(vec.end(), words);
    for (const auto& word: words) {
        by_word_map_[word].push_back(class_name);
    }
}

/** Call this after adding all words to clean up duplicates. */
void word_classes_lookup::finalize()
{
    for (auto it: by_word_map_) {
        auto& words_vec = it.second;
        std::sort(words_vec.begin(), words_vec.end());
        const auto last = std::unique(words_vec.begin(), words_vec.end());
        words_vec.erase(last, words_vec.end());
    }

    for (auto it: by_class_map_) {
        auto& words_vec = it.second;
        std::sort(words_vec.begin(), words_vec.end());
        const auto last = std::unique(words_vec.begin(), words_vec.end());
        words_vec.erase(last, words_vec.end());
    }
}

const word_classes_lookup::words_vec& word_classes_lookup::classes_for_word(const u16string& word) const
{
    const auto it = by_word_map_.find(word);
    if (it != by_word_map_.end()) {
        return it->second;
    }
    return empty_words_vec_;
}

const word_classes_lookup::words_vec& word_classes_lookup::words_in_class(const std::u16string& class_name) const
{
    const auto it = by_class_map_.find(class_name);
    if (it != by_class_map_.end()) {
        return it->second;
    }
    return empty_words_vec_;
}

} //end namespace cpaf::semantic
