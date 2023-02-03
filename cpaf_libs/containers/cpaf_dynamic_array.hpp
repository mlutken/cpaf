#ifndef CPAF_DYNAMIC_ARRAY_H
#define CPAF_DYNAMIC_ARRAY_H

#include <algorithm>
#include <limits>
#include <memory>
#include <cstring>
#include <cstddef>

namespace cpaf::containers
{

/** Dynamic array. Currently only support POD types.
    Meant as a wrapper around a plain C-style array, with memory management.
    Can be used as a fast buffer for example. */
template <typename T, std::size_t EXTRA_CAPACITY = 0>
class dynamic_array
{
public:
    using value_type = T;
    using size_type                 = size_t;
    using difference_type           = ptrdiff_t;
    using reference                 = value_type&;
    using const_reference           = const value_type&;
    using pointer                   = value_type*;
    using const_pointer             = const value_type*;

    using iterator                  = value_type*;
    using const_iterator            = const value_type*;
    using reverse_iterator          = std::reverse_iterator<iterator>;
    using const_reverse_iterator    = const std::reverse_iterator<const_iterator>;

    // ------------------------------
    // -- Constructors/Assignment ---
    // ------------------------------
    dynamic_array() = default;

    explicit dynamic_array(size_type count)
        : size_(0)
    {
        reserve_for_assign(count);
    }

    dynamic_array(size_type count, const T& value)
    {
        reserve_for_assign(count);
        if constexpr (EXTRA_CAPACITY == 1) {
            data_[count] = value_type{};
        }
        for (auto i = count; i > 0; ) {
            --i;
            data_[i] = value; // No need for placement new with POD types
            // TODO: Non-POD case
        }
        size_ = count;
    }

    dynamic_array(const dynamic_array& other)
    {
        if (&other == this) {
            return;
        }
        capacity_ = other.capacity();
        size_ = other.size();
        data_ = std::make_unique<T[]>(capacity_);
        std::memcpy(&data_[0], &other[0], (size() + EXTRA_CAPACITY)*sizeof (T));
    }

    dynamic_array (dynamic_array&& other) noexcept
    {
        if (&other == this) {
            return;
        }
        capacity_ = other.capacity();
        size_ = other.size_;
        data_.swap(other.data_);
    }

    /** Assignment from other dynamic_array.

    We use the common copy construct and swap idiom
    as shown here:
    @code
    MyClass& operator=(MyClass other)
    {
        swap(other);
        return *this;
    }
    @endcode
    OR here:
    MyClass& operator=(const MyClass& other)
    {
        MyClass tmp(other);
        swap(tmp);
        return *this;
    }
    This is because the swap function will not be very efficient given the
    static nature of this dynamic_array: swap needs to copy all elements and not
    just swap pointers as std::dynamic_array::swap() can do.
    */
    dynamic_array& operator=(dynamic_array&& other)
    {
        if (&other == this) {
            return *this;
        }
        capacity_ = other.capacity();
        size_ = other.size();
        data_.swap(other.data_);
        return *this;
    }

    dynamic_array& operator= (const dynamic_array& other)
    {
        if (&other == this) {
            return *this;
        }

        dynamic_array tmp(other);
        swap(tmp);
        return *this;
    }

    dynamic_array& operator= (std::initializer_list<T> ilist)
    {
        auto it = ilist.begin();
        const auto end = ilist.end();
        const auto count = check_range(it, end);
        reserve_for_assign(count);

        if constexpr (EXTRA_CAPACITY == 1) {
            data_[count] = value_type{};
        }

        size_type i = 0u;
        for (; it != end; ++it, ++i) {
            data_[i] = *it; // No need for placement new with POD types
            //new (insert_ptr) value_type(*it); // TODO: Non-POD case
        }
        size_ = count;
        return *this;
    }

    void assign(size_type count, const T& value)
    {
        reserve_for_assign(count);

        if constexpr (EXTRA_CAPACITY == 1) {
            data_[count] = value_type{};
        }
        // TODO: Use parallel std algorithm
        for (auto i = count; i > 0; ) {
            --i;
            data_[i] = value; // No need for placement new with POD types
            //new (insert_ptr) value_type(*it); // TODO: Non-POD case
        }
        size_ = count;
    }

    void assign (const value_type* other, size_type size_other)
    {
        reserve_for_assign(size_other);
        std::memcpy(&data_[0], &other[0], (size_other + EXTRA_CAPACITY) * sizeof(T));
        size_ = size_other;
    }

    template< class InputIt >
    void assign(InputIt first, InputIt last)
    {
        const auto count = check_range(first, last);
        reserve_for_assign(count);

        if constexpr (EXTRA_CAPACITY == 1) {
            data_[count] = value_type{};
        }

        size_type i = 0u;
        for (auto it = first; it != last; ++it, ++i) {
            data_[i] = *it; // No need for placement new with POD types
            //new (insert_ptr) value_type(*it); // TODO: Non-POD case
        }
        size_ = count;
    }

    void assign(std::initializer_list<T> ilist)
    {
        auto it = ilist.begin();
        const auto end = ilist.end();
        const auto count = check_range(it, end);
        reserve_for_assign(count);

        if constexpr (EXTRA_CAPACITY == 1) {
            data_[count] = value_type{};
        }

        size_type i = 0u;
        for (; it != end; ++it, ++i) {
            data_[i] = *it; // No need for placement new with POD types
            //new (insert_ptr) value_type(*it); // TODO: Non-POD case
        }
        size_ = count;
    }

    void reserve(size_type new_capacity)
    {
        auto new_cap = new_capacity;
        if (capacity() == 0) {
            new_cap += EXTRA_CAPACITY;
        }
        auto new_data = std::make_unique<T[]>(new_cap);
        std::memcpy(&new_data[0], &data_[0], size()*sizeof (T));

        capacity_ = new_cap;
        data_ = std::move(new_data);
    }

    void reserve_for_append(size_t elems_append_count)
    {
        const auto needed = size() + elems_append_count + EXTRA_CAPACITY;
        if (capacity() < needed) {
            auto new_capacity = needed;
            auto const double_capacity = capacity()*2;
            if (new_capacity < double_capacity) { // Ensure we at least double the capacity!
                new_capacity = double_capacity;
            }
            reserve(new_capacity);
        }
    }

    ~dynamic_array() {
        // destroy_elements(); TODO: Non-POD support!
    }

    // ----------------------
    // --- Element access ---
    // ----------------------
    reference at(size_type pos)
    {
        if (pos >= size()) {
            throw std::out_of_range{"cas::dynamic_array access (dynamic_array::at()) beyond size."};
        }
        return (*this)[pos];
    }

    const_reference at(size_type pos) const
    {
        if (pos >= size()) {
            throw std::out_of_range{"cas::dynamic_array access (const dynamic_array::at()) beyond size."};
        }
        return (*this)[pos];
    }

    reference       operator[]( size_type pos ) { return data_[pos]; }
    const_reference operator[]( size_type pos ) const { return data_[pos]; }

    reference       front() { return (*this)[0]; }
    const_reference front() const { return (*this)[0]; }

    reference       back() { return (*this)[size_ -1]; }
    const_reference back() const { return (*this)[size_ -1]; }

    T*              data() noexcept { return &(*this)[0]; }
    const T*        data() const noexcept { return &(*this)[0]; }

    T*              data_append_ptr() noexcept { return &(*this)[size()]; }
    const T*        data_append_ptr() const noexcept { return &(*this)[size()]; }

    // -----------------
    // --- Iterators ---
    // -----------------
    iterator                begin   ()  noexcept { return &(*this)[0]; }
    const_iterator          begin   ()  const noexcept { return &(*this)[0]; }
    const_iterator          cbegin  ()  const noexcept { return &(*this)[0]; }

    iterator                end     ()  noexcept { return &(*this)[size_]; }
    const_iterator          end     ()  const noexcept { return &(*this)[size_]; }
    const_iterator          cend    ()  const noexcept { return &(*this)[size_]; }

    reverse_iterator        rbegin  ()  noexcept { return reverse_iterator(end()); }
    const_reverse_iterator  rbegin  ()  const noexcept { return const_reverse_iterator(cend()); }
    const_reverse_iterator  crbegin ()  const noexcept { return const_reverse_iterator(cend()); }

    reverse_iterator        rend    ()  noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator  rend    ()  const noexcept { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator  crend   ()  const noexcept { return const_reverse_iterator(cbegin()); }

    // -----------------------
    // --- Capacity / Size ---
    // -----------------------
    bool            empty           () const noexcept {   return size_ == 0; }
    size_type       size            () const noexcept {   return size_; }
    void            size_force      (size_type count) noexcept { size_ = count; }
    size_type       max_size        () const noexcept {   return std::numeric_limits<size_type>::max(); }
    size_type       capacity        () const noexcept {   return capacity_; }
    size_type       rest_capacity   () const noexcept {   return capacity() - size(); }

    // -----------------
    // --- Modifiers ---
    // -----------------
    void push_back (const value_type& elem)
    {
        const auto needed = size() + 1 + EXTRA_CAPACITY;
        if (capacity() < needed) {
            auto new_capacity = needed;
            auto const double_capacity = capacity()*2;
            if (new_capacity < double_capacity) {
                new_capacity = double_capacity;
            }
            reserve(new_capacity);
        }

        data_[size()] = elem;
        ++size_;

        if constexpr (EXTRA_CAPACITY == 1) {
            data_[size()] = value_type{};
        }
    }

    void append (const value_type& elem)
    {
        push_back(elem);
    }

    void append (const dynamic_array& other)
    {
        reserve_for_append(other.size());
        std::memcpy(&data_[size()], &other[0], (other.size() + EXTRA_CAPACITY) * sizeof(T));
        size_ += other.size();
    }

    void append (const value_type* other, size_type size_other)
    {
        reserve_for_append(size_other);
        std::memcpy(&data_[size()], &other[0], (size_other + EXTRA_CAPACITY) * sizeof(T));
        size_ += size_other;
    }

    void clear() noexcept
    {
        capacity_ = 0u;
        size_ = 0u;
        data_.reset();
    }

    // ------------------------
    // --- Helper functions ---
    // ------------------------
private:
    void reserve_for_assign (size_type count)
    {
        const auto needed_capacity = count + EXTRA_CAPACITY;
        if (needed_capacity > capacity()) {
            capacity_ = needed_capacity;
            data_ = std::make_unique<T[]>(capacity());
        }
    }

    /** Check range and return count of range. */
    template<class InputIt>
    size_type check_range(InputIt first, InputIt last)
    {
        const auto diff_signed = last - first;
        if (diff_signed < 0) {
            throw std::out_of_range{"cas::vector_s range constructing/assigning from inverted range."};
        }
        return static_cast<size_type>(diff_signed);
    }

    /** Shift range right by n elements. */
    template<class InputIt>
    void shift_right(InputIt first, InputIt last, size_type n)
    {
        const auto diff_signed = last - first;
        if (diff_signed < 0) {
            return;
        }

        // Make room for elements
        for (size_type i = 0; i < n; ++i) {
            const pointer insert_ptr = last + i;
            new (insert_ptr) value_type{}; // Default contruct the extra elements needed. Could perhaps construct when first needed instead!
        }

        for (auto it = last; it != first;) {
            --it;
            *(it+n) = std::move(*it);
        }
    }

    /** Shift range left by n elements. */
    template<class InputIt>
    void shift_left(InputIt first, InputIt last, size_type n)
    {
        const auto diff_signed = last - first;
        if (diff_signed < 0) {
            return;
        }
        for (auto it = first; it != last;) {
            *(it-n) = std::move(*it);
            ++it;
        }
    }

private:
    // --- Member data --
    std::unique_ptr<T[]>    data_;
    size_type               capacity_   = 0u;
    size_type               size_       = 0u;
};

// ------------------------
// -- Compare operators ---
// ------------------------
template <typename T>
bool operator==(const dynamic_array<T>& lhs, const dynamic_array<T>& rhs)
{
    if (lhs.size() != rhs.size()) {
        return false;
    }
    return std::memcmp(lhs.data(), rhs.data(), lhs.size()*sizeof(T));
}

template <typename T>
bool operator!=(const dynamic_array<T>& lhs, const dynamic_array<T>& rhs)
{
    return !(lhs == rhs);
}

// --- Useful Typedefs ---
using dynamic_byte_array    = dynamic_array<std::byte>;
using dynamic_char_array    = dynamic_array<char>;
using dynamic_uchar_array   = dynamic_array<unsigned char>;

} //end namespace cpaf::containers


#endif //CPAF_DYNAMIC_ARRAY_H

