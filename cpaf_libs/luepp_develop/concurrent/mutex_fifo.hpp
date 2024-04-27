#pragma once

#include <mutex>
#include <concurrent/srsw_fifo.hpp>

namespace estl {


/**
Mutex protected fifo.
@sa estl::srsw_fifo which is the same using a lockless approach, but only
    supporting one reader and one writer.
*/
template <typename T, class Allocator = std::allocator<T>, size_t ALIGN_SIZE = 128 >
class mutex_fifo
{
public:
    using underlying_fifo_t =  estl::srsw_fifo<T, Allocator, ALIGN_SIZE>;
    // ------------------------
    // --- PUBLIC: Typedefs ---
    // ------------------------
    using value_type = typename underlying_fifo_t::value_type;
    using size_type = typename underlying_fifo_t::size_type;
    using atomic_size_type = typename underlying_fifo_t::atomic_size_type;
    using allocator_type = typename underlying_fifo_t::allocator_type;
    using difference_type = typename underlying_fifo_t::difference_type;
    using reference = typename underlying_fifo_t::reference;
    using const_reference = typename underlying_fifo_t::const_reference;
    using pointer = typename underlying_fifo_t::pointer;
    using const_pointer = typename underlying_fifo_t::const_pointer;

    mutex_fifo ()
        : fifo_(),
          mutex_()
    {
    }

    explicit mutex_fifo (size_type queue_size)
        : fifo_(queue_size),
          mutex_()
    {
    }

    mutex_fifo (size_type queue_size, const allocator_type& allocator)
        : fifo_(queue_size, allocator),
          mutex_()
    {
    }

    void init_queue(size_type queue_size) {
        std::scoped_lock<std::mutex> lock(mutex_);
        fifo_.init_queue(queue_size);
    }

    void resize_queue(size_type queue_size) {
        std::scoped_lock<std::mutex> lock(mutex_);
        fifo_.resize_queue(queue_size);
    }

    bool push (T&& v)
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        if (fifo_.full()) {
            fifo_.resize_queue(fifo_.size()*2);
        }
        return fifo_.push(std::move(v));
    }

    template<class... Args >
    bool emplace(Args&&... args)
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        if (fifo_.full()) {
            fifo_.resize_queue(fifo_.size()*2);
        }
        return fifo_.emplace(std::forward<Args>(args)...);
    }

    bool push (const T& v)
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        if (fifo_.full()) {
            fifo_.resize_queue(fifo_.size()*2);
        }
        return fifo_.push(v);
    }


    bool pop ()
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return fifo_.pop();
    }

    T pop_front ()
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        if (fifo_.empty()) {
            return T{};
        }
        T elem = std::move(fifo_.front());
        fifo_.pop();
        return elem;
    }

    bool pop_front (T& elem)
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        if (fifo_.empty()) {
            return false;
        }
        elem = std::move(fifo_.front());
        fifo_.pop();
        return true;
    }

    T& front ()
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return fifo_.front();
    }

    const T& front () const
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return fifo_.front();
    }

    void flush () {
        std::scoped_lock<std::mutex> lock(mutex_);
        fifo_.flush();
    }

    bool full () const
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return fifo_.full();
    }

    bool empty () const
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return fifo_.empty();
    }

    size_type size() const
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return fifo_.size();
    }

    size_type buffer_size() const
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return fifo_.buffer_size();
    }

    size_type capacity() const
    {
        std::scoped_lock<std::mutex> lock(mutex_);
        return fifo_.capacity();
    }

private:
    // -----------------------------
    // PRIVATE: Helper functions ---
    // -----------------------------

    // ------------------------
    // PRIVATE: Member data ---
    // ------------------------
    estl::srsw_fifo<T, Allocator, ALIGN_SIZE>   fifo_;
    alignas(ALIGN_SIZE) mutable std::mutex      mutex_;   // Aligning to avoid "false sharing"
};


} // END namespace estl
