#pragma once
#include "stdafx.h"

#include <vector>
#include <cstdint>

// --------------------
// --- ModuloFifo.h ---
// --------------------

template <typename T>
struct DefaultModuloPolicy
{
  unsigned int  getModuloVal( const T& val ) const { return static_cast<unsigned int>(val); }
};


template <typename T, template <typename T, class Allocator> class FifoClass, class ModuloPolicy, class Allocator = std::allocator<T> >
class ModuloFifo
{
private:
  typedef FifoClass<T, Allocator >    Fifo_t;

public:
  // ------------------------
  // --- PUBLIC: Typedefs ---
  // ------------------------
  typedef T                                     value_type;
  typedef uint32_t                              size_type;
  typedef Allocator                             allocator_type;
  typedef typename Fifo_t::difference_type      difference_type;
  typedef typename Fifo_t::reference            reference;
  typedef typename Fifo_t::const_reference      const_reference;
  typedef typename Fifo_t::pointer              pointer;
  typedef typename Fifo_t::const_pointer 	      const_pointer;

  ModuloFifo ( size_type queueSize, size_type moduloBase )
    : m_fifos( moduloBase, 0 )
    , m_moduloPolicy()
  {
    for ( int i = 0 ; i < m_fifos.size() ; ++i )
    {
      m_fifos[i] = new Fifo_t(queueSize);
    }
  }

  ModuloFifo ( size_type queueSize, size_type moduloBase, const allocator_type& allocator )
    : m_fifos( moduloBase, 0 )
    , m_moduloPolicy()
  {
    for ( int i = 0 ; i < m_fifos.size() ; ++i )
    {
      m_fifos[i] = new Fifo_t(queueSize, allocator);
    }
  }

  ~ModuloFifo ()
  {
    for ( int i = 0 ; i < m_fifos.size() ; ++i )
    {
      delete m_fifos[i];
    }
  }

  bool  push ( const T& v )
  {
    size_type queueIndex = queueIndexGet(v);
    return m_fifos[queueIndex]->push(v);
  }


  bool  pop ( size_type queueIndex )
  {
    return m_fifos[queueIndex]->pop();
  }

  T&  front ( size_type queueIndex )
  {
    return m_fifos[queueIndex]->front();
  }

  bool full ( size_type queueIndex ) const
  {
    return  m_fifos[queueIndex]->full();
  }

  bool empty ( size_type queueIndex ) const
  {
    return  m_fifos[queueIndex]->empty();
  }

  size_type moduloBase() const { return m_fifos.size(); }

private:
  // ---------------------
  // PRIVATE: Typedefs ---
  // ---------------------
  typedef std::vector< Fifo_t* >       FifoVec_t;

  // -----------------------------
  // PRIVATE: Helper functions ---
  // -----------------------------
  ModuloFifo(ModuloFifo const&);
  ModuloFifo& operator=(ModuloFifo const&);        

  size_type    queueIndexGet ( const T& v ) const 
  {
    size_type moduloVal = m_moduloPolicy.getModuloVal(v);
    return moduloVal % m_fifos.size(); 
  } 

  // ------------------------
  // PRIVATE: Member data ---
  // ------------------------
  FifoVec_t         m_fifos;
  ModuloPolicy      m_moduloPolicy;

};
