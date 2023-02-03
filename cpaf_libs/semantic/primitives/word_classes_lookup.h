#ifndef CPAF_WORD_CLASSES_LOOKUP_H
#define CPAF_WORD_CLASSES_LOOKUP_H

#include <string>
#include <string_view>
#include <memory>
#include <array>
#include <map>
#include <unordered_map>
//#include <sparsehash/dense_hash_map>

#include <semantic/tokens/token_attributes.h>
#include <semantic/primitives/char_match.h>
#include <unicode/cpaf_unicode_chars.hpp>

// https://github.com/sparsehash/sparsehash-c11, https://github.com/sparsehash/sparsehash
// https://www.youtube.com/watch?v=fHNmRkzxHWs

namespace cpaf::semantic
{
/** Keeps track of words belong to different classes.
For example we could make a class called "{volume}" and then add words like this:
@example
my_word_classes_lookup.add(u"volume", {u"liters", u"l", u"ml"}); // etc...
@endexample
@example
my_word_classes_lookup.add(u"preposition", {u"for", u"at", u"on"}); // etc...
@endexample

*/
class word_classes_lookup {
public:
    using words_view_vec    = std::vector<std::u16string_view>;
    using words_vec         = std::vector<std::u16string>;

    static bool is_class_name   (const std::string_view& str);

    word_classes_lookup() = default;

    void                add             (const std::u16string& class_name, const std::u16string& word);
    void                add             (const std::u16string& class_name, std::initializer_list<std::u16string> words);
    void                finalize        ();
    const words_vec&    classes_for_word(const std::u16string& word) const;
    const words_vec&    words_in_class  (const std::u16string& class_name) const;

    // --- Member data ---
private:
    using word_map  = std::unordered_map<std::u16string, words_vec>;
    word_map        by_word_map_;   // All classes a given word belongs to lookup.
    word_map        by_class_map_;  // All words belonging to a class lookup. @todo Maybe we don't need this after all!!
    words_vec       empty_words_vec_;   // For returning when words_in_class finds nothing
};


} //end namespace cpaf::semantic


#endif //CPAF_WORD_CLASSES_LOOKUP_H

