#pragma once
#include "stdafx.h"

#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/cstdint.hpp>

typedef boost::shared_mutex                         MutexT;
typedef boost::unique_lock< boost::shared_mutex >   UniqueLockT;
//typedef DC::Shared::CriticalSection               MutexT;
//typedef DC::Shared::CriticalSection::Lock         UniqueLockT;


// ---------------------------------------
// --- MutipleWriterSingleReaderFifo.h ---
// ---------------------------------------


/** 
Fifo to support an (unspecified) number of writers/producers and a single reader/consumer.
The write ( push(elem) ) functions blocks other writes but not the reader. */
template <typename T, class Allocator = std::allocator<T> >
class MutipleWriterSingleReaderFifo
{
private:
  typedef std::vector<T, Allocator >    QueueVec_t;
public:
  // ------------------------
  // --- PUBLIC: Typedefs ---
  // ------------------------
  typedef T                                     value_type;
  typedef boost::uint32_t                       size_type;
  typedef Allocator                             allocator_type;
  typedef typename QueueVec_t::difference_type  difference_type;
  typedef typename QueueVec_t::reference        reference;
  typedef typename QueueVec_t::const_reference  const_reference;
  typedef typename QueueVec_t::pointer          pointer;
  typedef typename QueueVec_t::const_pointer 	const_pointer;

  explicit MutipleWriterSingleReaderFifo ( size_type queueSize )
    : m_queue(queueSize)
    , m_writeIndex(0)
    , m_readIndex(0)
  {

  }

  MutipleWriterSingleReaderFifo ( size_type queueSize, const allocator_type& allocator )
    : m_queue(queueSize, allocator)
    , m_writeIndex(0)
    , m_readIndex(0)
  {

  }

  bool  push ( const T& v )
  {
    UniqueLockT lock(m_writeGuard);
    size_type nextWriteIndex = incIndex(m_writeIndex);
    if ( nextWriteIndex != m_readIndex ) {
      m_queue[m_writeIndex] = v;
      m_writeIndex = nextWriteIndex;
      return true;
    }
    return false;
  }


  bool  pop ()
  {
    if ( empty () ) return false;
    m_readIndex = incIndex (m_readIndex);
    return true;
  }

  T&  front ()
  {
    return m_queue[m_readIndex];
  }

  bool full () const
  {
    size_type writeCheck = incIndex(m_writeIndex);
    return (writeCheck == m_readIndex);
  }

  bool empty () const
  {
    return (m_writeIndex == m_readIndex);
  }


private:
  // ---------------------
  // PRIVATE: Typedefs ---
  // ---------------------
  typedef boost::shared_mutex                         MutexT;
  typedef boost::unique_lock< boost::shared_mutex >   UniqueLockT;
  //typedef DC::Shared::CriticalSection               MutexT;
  //typedef DC::Shared::CriticalSection::Lock         UniqueLockT;

  // -----------------------------
  // PRIVATE: Helper functions ---
  // -----------------------------
  MutipleWriterSingleReaderFifo(MutipleWriterSingleReaderFifo const&);
  MutipleWriterSingleReaderFifo& operator=(MutipleWriterSingleReaderFifo const&);        

  //bool    tryIncWriteIndex ( size_type& oldWriteIndex ) 
  //{ 
  //  UniqueLockT lock(m_incWriteIndexGuard);
  //  oldWriteIndex = m_writeIndex;
  //  size_type nextWriteIndex = incIndex(m_writeIndex);
  //  if ( nextWriteIndex != m_readIndex ) {
  //    m_writeIndex = nextWriteIndex;
  //    return true;
  //  }
  //  return false; 
  //} 


  size_type    incIndex ( size_type index ) const { return (index +1) % m_queue.size(); } 

  // ------------------------
  // PRIVATE: Member data ---
  // ------------------------
  volatile size_type  m_writeIndex;
  QueueVec_t          m_queue;      // Queue declared between the two indexes which hopefully avoids "false sharing"
  mutable MutexT      m_writeGuard;
  volatile size_type  m_readIndex;

};

