#include "skipped_tokens.h"
#include <iostream>
#include <unicode/cpaf_u16string_utils.h>

using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;

namespace cpaf::semantic
{

string skipped_tokens::iter_pair::dbg_str() const
{
    return cpaf::unicode::to_string(dbg_u16str());
}


skipped_tokens::skipped_tokens()
{
}

void skipped_tokens::clear()
{
    skipped_ranges_.clear();
    new_parse();
}

void skipped_tokens::new_parse()
{
    try_order_ = try_order::try_start;
    current_parse_start_size_ = skipped_ranges_.size();
}

void skipped_tokens::update(const skipped_tokens::const_iterator& cur_token_it, try_order order)
{
    if (cur_token_it->attributes().is_whitespace()) {
        return;
    }
    if (skip_currently_active()) {
        update_current_active(cur_token_it, order);
    }
    else if (is_try_order_skip(order)) {
        update_start_new_active(cur_token_it, order);
    }



    try_order_ = order;
}

void skipped_tokens::drop_ranges_from_this_parse()
{
    while (skipped_ranges_.size() > current_parse_start_size_) {
        skipped_ranges_.pop_back();
    }
}

void skipped_tokens::debug_print() const
{
    if (skipped_ranges_.empty()) return;
    std::cerr << "--- Ranges ---\n";
    for (const iter_pair& range : skipped_ranges_) {
        std::cerr << "Range: '" << u16_view(range.first, range.second) << "'\n";
    }
    std::cerr << "-----------\n";
}

bool skipped_tokens::is_try_order_skip(try_order order) const
{
    return order == try_order::any_word ||
           order == try_order::wildcard;
}

bool skipped_tokens::skip_currently_active() const
{
    return is_try_order_skip(try_order_);
}

void skipped_tokens::update_start_new_active(
        const const_iterator& cur_token_it,
        try_order /*order*/)
{
    start_new_range(cur_token_it, cur_token_it +1);
}

void skipped_tokens::update_current_active(
        const const_iterator& cur_token_it,
        try_order order)
{
    if (is_try_order_skip(order)) {
        update_active_range_end(cur_token_it +1);
    }
    else {
        end_current_curent_active();
    }
}

void skipped_tokens::start_new_range(
        const const_iterator& begin,
        const const_iterator& end)
{
    skipped_ranges_.emplace_back(begin, end);
}

void skipped_tokens::update_active_range_end(const const_iterator& end)
{
    if (skipped_ranges_.empty()) return;
    skipped_ranges_.back().second = end;
}

void skipped_tokens::end_current_curent_active()
{
    try_order_ = try_order::try_start;
    if (skipped_ranges_.empty()) return;
    skipped_ranges_.back().range_completed_with_skipped_tokens = true;
}


} //end namespace cpaf::semantic


