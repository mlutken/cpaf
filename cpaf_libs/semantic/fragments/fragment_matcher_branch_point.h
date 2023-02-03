#ifndef CPAF_FRAGMENT_MATCHER_BRANCH_POINT_H
#define CPAF_FRAGMENT_MATCHER_BRANCH_POINT_H

#include <cpaf_libs/semantic/fragments/fragment_try_order.h>
#include <cpaf_libs/semantic/tokens/token.h>
namespace cpaf::semantic
{

struct fragment_matcher_branch_point
{
    explicit    fragment_matcher_branch_point(const token::vector::const_iterator& lookup_iterator)
        : try_order_index_(try_order::try_start),
          token_iterator_(lookup_iterator)
    {
    }

    void            advance_try_order                   ()          { ++try_order_index_; }
    try_order       try_order_index                     () const    { return try_order_index_; }

    const token::vector::const_iterator&    token_iterator () const { return token_iterator_; }

    bool done () const
    {
        return try_order_index_ == try_order::try_end;
    }

    try_order                       try_order_index_;
private:
    token::vector::const_iterator   token_iterator_;
};

} //end namespace cpaf::semantic


#endif //CPAF_FRAGMENT_MATCHER_BRANCH_POINT_H

