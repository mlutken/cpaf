#ifndef CPAF_SKIPPED_TOKENS_H
#define CPAF_SKIPPED_TOKENS_H

#include <cpaf_libs/semantic/fragments/fragment_try_order.h>
#include <semantic/tokens/token.h>

namespace cpaf::semantic
{

class skipped_tokens
{
public:
    using const_iterator    = std::vector<token>::const_iterator;

    struct iter_pair {
        iter_pair() = default;
        iter_pair(const const_iterator& f, const const_iterator& s)
            : first(f)
            , second(s)
        {
        }

        std::pair<const_iterator, const_iterator> std_pair() const {
            return std::make_pair(first, second);
        }

        std::u16string     dbg_u16str() const
        {
            return std::u16string(first->begin(), second->end());
        }

        std::string     dbg_str() const;

        const_iterator  first;
        const_iterator  second;
        bool            range_completed_with_skipped_tokens = false;
    };

    using iter_pair_vec     = std::vector<iter_pair>;

    skipped_tokens();

    const iter_pair_vec&                ranges() const { return skipped_ranges_; }

    void    clear                       ();
    void    new_parse                   ();
    void    update                      (const const_iterator& cur_token_it, try_order order);
    void    drop_ranges_from_this_parse ();
    bool    skip_currently_active       () const;

    void    debug_print                 () const;
private:
    bool    is_try_order_skip           (try_order order) const;
    void    update_start_new_active     (const skipped_tokens::const_iterator& cur_token_it, try_order order);
    void    update_current_active       (const skipped_tokens::const_iterator& cur_token_it, try_order order);

    void    start_new_range             (const const_iterator& begin, const const_iterator& end);
    void    update_active_range_end     (const const_iterator& end);
    void    end_current_curent_active   ();

private:
    iter_pair_vec               skipped_ranges_;
    iter_pair_vec::size_type    current_parse_start_size_   = 0;
    try_order                   try_order_                  = try_order::try_start;
};


} //end namespace cpaf::semantic


#endif //CPAF_SKIPPED_TOKENS_H

