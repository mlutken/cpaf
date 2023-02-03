#ifndef CPAF_FRAGMENT_TRY_ORDER_H
#define CPAF_FRAGMENT_TRY_ORDER_H

#include <cstddef>
#include <type_traits>

namespace cpaf::semantic
{
enum class try_order : size_t {
    try_start=0,
    plain_word,
    classified_word,
    word_pattern,
    number,
    any_word,
    wildcard,
    try_end
};

inline try_order& operator++ (try_order& value) {
	if (value == try_order::try_end) {
		return value;
	}
    using int_type = typename std::underlying_type<try_order>::type;
	value = static_cast<try_order>(static_cast<int_type>(value) + 1);
	return value;
}

} //end namespace cpaf::semantic


#endif //CPAF_FRAGMENT_TRY_ORDER_H

