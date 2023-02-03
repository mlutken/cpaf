#ifndef _MY_CONTAINER_HPP_
#define _MY_CONTAINER_HPP_
// ****************************************************************************
// CREATED_ON	: 09-07-03
// CREATED_BY	: Martin Lutken [mlutken@bigfoot.com]
// ****************************************************************************
#include <memory>

#include "stl_boost.h"

#include "../tutils/rel_ops.hpp"			// Compiler generation of relational operators.
#include "../tutils/typemanip.hpp"		// The select template.

_STL_BOOST_BEGIN_NAMESPACE


using tutils::rel_ops::operator!=;
//using namespace tutils::rel_ops;	// If you need all operators from rel_ops



/**
It seems very hard indeed to find anything on the net or in C++ STL books
about how to implement a fully compliant STL container with class iterators, 
how to use the allocator etc. So I've made this easy to understand example, 
which should more or less show it all.

This demonstrates how to make STL-like container classes with custom iterator
classes as weel as how to use the template allocator type.
The container type we model here is a single linked list. However only
a subset of the "real" STL::slist is implemented here!
The focus is on making the iterator and const_iterator work as they are supposed to,
which can be quite tricky. I have peeked a little in MS.NET2003's and STLPort's
STL implementations, but they can be quite hard to read ... nevertheless it's
possible to get some inspiration. I've also had some inspiration from the loki
library, described in "Modern C++ Design" by Andrei Alexandrescu. This book
inspired to my iterator implementation which actually looks simpler than the
one in fx. MS.NET2003's <list> implementation.
Only thing I'm not quite clear about is how to deal with a declaration of a 
container like:
typedef my_container<const int>	MyCon;
MS.NET's STL seems to ignore the const completely. STLPort doesn't handle a 
constructor like: MyCon c(3,10); with the above declaration.
The my_container accepts the above constructor as well as additional push_front
calls, but disallow modifications to elements in the container. The more "normal"
situation with a const container declaration like: 	
typedef const my_container<T>		MyCon;
Works as the other STL implementations!
Exceptions are not in this example, but should be easy to add.

*/

template <typename T, class A = std::allocator<T> >	// IMPORTANT: the spaces between the '>'s are necessary in order to compile. Well known "defect" of C++ :-(.
class my_container 
{
public:

	// -----------------------
	// --- Typedefs PUBLIC ---
	// -----------------------

	typedef A								allocator_type;	 
	typedef typename	A::value_type		value_type;			///< The type of object, T, stored in the vector.
	typedef typename	A::reference		reference;			///< Reference to T.
	typedef typename	A::const_reference	const_reference;	///< Const reference to T.
	typedef typename	A::pointer			pointer;			///< Pointer to T.			
	typedef typename	A::const_pointer	const_pointer;		///< Pointer to const T. CHECK: Is this correct?
	typedef typename	A::size_type		size_type;			///< An unsigned integral type.			
	typedef typename	A::difference_type	difference_type;	///< A signed integral type.



	// ------------------------------------
	// --- NodeType Inner Class PRIVATE ---
	// ------------------------------------
private:
	/// TODO: Describe this
	struct my_container_node
	{
		// --- Typedefs ---
		typedef	my_container_node							node_value_type;

		/** The rebind member allows a container to construct an allocator for some
		arbitrary data type. The allocator type is determined by the template
		parameter. For example, a container may need to allocate types other than
		T (i.e. list nodes or hash buckets). In this case the container could
		obtain correct type, typically with a typedef: 
		\see the allocator example "my_allocator.hpp" in this directory.*/
		typedef typename A::rebind<node_value_type>::other	node_allocator;
		typedef typename node_allocator::pointer			node_pointer;		///< Pointer to our nested node type.
		typedef typename node_allocator::const_pointer		node_const_pointer;	///< Const pointer to our nested node type.

		/** Construct from T element. */
		explicit my_container_node(const T& elem)
			: m_pNext(0), m_Data(elem) {};

		// --- Member data ---
		node_pointer				m_pNext;
		T							m_Data;

	};

	// ----------------------------------
	// --- Node type typedefs PRIVATE ---
	// ----------------------------------
	typedef typename my_container_node						node_value_type;

	typedef typename node_value_type::node_allocator		node_allocator;
	typedef typename node_value_type::node_pointer			node_pointer;
	typedef typename node_value_type::node_const_pointer	node_const_pointer;


	// --------------------------------------
	// --- Iterators Inner classes PUBLIC ---
	// --------------------------------------
public:

	/** Base iterator template class. Bsed on this two typedefs are made: one
		for const_iterator and one for iterator. This way we make sure we only 
		have to overload the iterator operators (like '++' '==' etc.) one place
		unlike fx. MS.NET's STL implementation which makes a 'const_iterator' with
		all overloads and then derive to 'iterator' and repeat allmost all overloads
		because most of the overloads return types differs in the two cases. Here
		we just let the compiler do the hard work by using a clever template.*/
	template <bool bIsConst> 
	class iterator_base
	{
		/** FRIEND TO OURSELVES. We need to be "our own friend" because different
			instantiations of this class leads to distinct types and then the 
			compiler won't let us get away with accessing private data like it's
			done in eg. the constructors. */
		friend class iterator_base;
		friend class my_container;
	public:
		// --- Typedefs PUBLIC ---
		typedef typename	std::forward_iterator_tag	iterator_category;	///< Iterator category. Certain algorithms need certain kinds of iterators.
		typedef typename	A::value_type				value_type;			///< The type of object, T, pointed to by the iterator.
		typedef typename	A::difference_type			difference_type;	///< CHECK: Not needed unless for random iterators.

		/// Select pointer or const_pointer to T.
		typedef typename	tutils::select<bIsConst, const_pointer, pointer>::result			pointer;	

		/// Select reference or const_reference to T.
		typedef typename	tutils::select<bIsConst, const_reference, reference>::result		reference;	

		/// Select node_pointer or node_const_pointer.
		typedef typename	tutils::select<bIsConst, node_const_pointer, node_pointer>::result	node_pointer;	



		// --- Constructors PUBLIC ---

		/// Default constructor. Initialize node pointer to 0.
		iterator_base() : m_pNode(0) { }

		/// Contruct from node pointer. Initialize node pointer to pNode.
		iterator_base(node_pointer pNode)					///< [in] Pointer to my_container_node.
			: m_pNode(pNode) { }

		/** Copy constructor template. Construct from general 
			iterator_base<REFERENCE, NODE_POINTER> */
		template <bool bIsCon> 
		iterator_base(const iterator_base<bIsCon>& it)		///< [in] Iterator to construct from.
			: m_pNode(it.m_pNode) { }

		
		// --- Operator overloads PUBLIC ---

		/** Acces operator '*' de-reference. Pointer dereference to const or 
			mutable object T. */
		reference			operator*() const 
		{
			return m_pNode->m_Data;	// Return designated value.
		}

		/// Acces operator '->'. 
		pointer				operator->() const 
		{	// Delegate to operator '*'
			return (&**this);	// Return pointer to class object
		}

		/** Prefix increment operator. Note we return by REFERENCE here. */
		iterator_base&		operator++()
		{
			m_pNode = m_pNode->m_pNext;		// Increment
			return *this;					// Return incremented iterator
		}

		/** Postfix increment operator. Note we return by VALUE here. The return
			here is declared as 'const' to prevent expressions such as 'myIterator++++'
			from compiling.*/
		const iterator_base	operator++(int)
		{
			iterator_base itTemp(*this);	// Make copy of original
			m_pNode = m_pNode->m_pNext;		// Increment
			return itTemp;					// Return copy of original
		}


		/** Equality test of iterators. Since the iterator_base class is a template 
			we make this operator (and possible other BINARY operators) a function
			template. This is because different instantiations of this class leads 
			to distinct types and we want '==' to work between ALL of them. 
			But instead of writing 4 versions of '==', which all does essentially
			the same we take the lazy approach an let the compiler do the work.
			The 4 '!=' is conviniently handled by the:
			"using tutils::rel_ops::operator!=;" statement in the top of this file.
			\see the 'tutils/rel_ops.hpp' file for more explanation. */
		template <bool bIsCon> 
		bool				operator==(	const iterator_base<bIsCon>& itR)	const ///< [in] Right operand.    
		{
			return ( this->m_pNode == itR.m_pNode );		// Compare pointers
		}

	private:

		node_pointer		get_node_pointer() {return m_pNode;}

		/** The type of m_pNode is either 'node_pointer' or 'node_const_pointer'
			depending on wheter this class is typedef'ed as (mutable) iterator (pointee
			object can BE modified)	or (non mutable) const_iterator (pointee object
			can NOT be modified).
			This ensures fx. that one can assign a mutable 'iterator' to an non mutable 
			const_itarator, but NOT vice versa. Ensured be the fact that you can assign
			a 'pObj*' to a 'const pObj*' but NOT the other way round! */
		node_pointer		m_pNode;	 
	};

	
	// --- ITERATOR TYPEDEFS ---

	/** Iterator. Used to iterate through a container. This is the normal mutable 
		iterator which allows modifying the pointed to object T.*/
	typedef iterator_base<false>	iterator;


	/** Const iterator. Used to iterate through a container. Not able to modify 
		the pointed to object T. */
	typedef iterator_base<true>		const_iterator;


	// ---------------------------------------------------
	// --- begin() end() front() back() methods PUBLIC ---
	// ---------------------------------------------------

	/// Return iterator using pointer to first element for iterator constructor.
	iterator			begin()			{ return iterator(m_pFirst);}
	const_iterator		begin() const	{ return const_iterator(m_pFirst);}

	/** Return iterator using pointer to one past last element for iterator 
		constructor. That means end() points to one past the last element, which 
		is just what the STL standard requires.  m_pLast->m_pNext is a NULL pointer.*/
//	iterator			end()			{return iterator(m_pLast->m_pNext);}
//	const_iterator		end() const		{return const_iterator(m_pLast->m_pNext);}
	iterator			end()			{return iterator(0);}			// This works just as fine :-)
	const_iterator		end() const		{return const_iterator(0);}		// This works just as fine :-)


	// ------------------------------------------
	// --- Constructors and destructor PUBLIC ---
	// ------------------------------------------
	/// Default constructor. Create empty container.
	my_container():	m_pFirst(0), m_pLast(0)	{};
	
	/** Create container with n elements initialized with their default constructor.
		WE USE keyword 'explicit' in order to prevent this constructor to do 
		implicit type conversions from size_type to my_container at places where 
		we don't wan't it.*/
	explicit my_container(size_type n)
		: m_pFirst(0), m_pLast(0)
	{
		while(n)
		{
			push_front(T());
			--n;
		}
	};


	/** Create container with n elements initialized with 'elem' .*/
	my_container(size_type n, const_reference elem)
		: m_pFirst(0), m_pLast(0)
	{
		while(n)
		{
			push_front(T(elem));
			--n;
		}
	};

	/** Create with allocator. Construct empty container, allocator. */
	explicit my_container(const allocator_type& alloc)
		: m_NodeAlloc(alloc), m_pFirst(0), m_pLast(0) {}
	

	/** Copy constructor TODO: Not implemented. It should be quite easy though.*/
	my_container(reference Container)
	{
		*this				= Container;	// Use '=' overload to implement copy constructor
	};


	/** Destructor. Clears the linked list by destroying all the elements. */
	~my_container() 
	{
		clear();
	};

	// -----------------------------
	// --- Access methods PUBLIC ---
	// -----------------------------

	/// Returns the first element. 
	reference front()					{return m_pFirst->m_Data; }		// For mutable container
	const_reference front() const		{return m_pFirst->m_Data; }		// For NON mutable container.

	/// Returns the last element. 
	reference back()					{return m_pLast->m_Data; }		// For mutable container
	const_reference back() const		{return m_pLast->m_Data; }		// For NON mutable container.


	// ----------------------------
	// --- Misc. methods PUBLIC ---
	// ----------------------------


	/// Get the allocator
	allocator_type get_allocator() const { return m_NodeAlloc; }


	// -------------------------------------
	// --- Modify content methods PUBLIC ---
	// -------------------------------------


	/// Insert element in fron of list.
	void push_front(const_reference elem)
	{
		if ( empty() )
		{
			m_pFirst	= create_node(elem);
			m_pLast		= m_pFirst;
		}
		else
		{
			node_pointer pOldFirst = m_pFirst;
			m_pFirst = create_node(elem);
			m_pFirst->m_pNext = pOldFirst;
		}
	}

	/// Remove top element. Container must not be empty before this call.
	void pop_front() 
	{
		node_pointer pDelete	= m_pFirst;
		m_pFirst				= m_pFirst->m_pNext;
		delete_node(pDelete);
	}

	
	/** Erases the element at position itPos. 
		\return An iterator to the element immediately following the one that was erased. */
	iterator erase(iterator itPos)					///< [in] Iterator to element to remove. 
	{
		if ( itPos.get_node_pointer() == m_pFirst )
		{
			pop_front();
			return begin();
		}
		else
		{
			node_pointer pPrev = find_previous(*itPos, m_pFirst);
			return iterator(erase_after(pPrev));
		}
	}

	/*	Erases the range [itFirst, itLast).
		\return  An iterator to the element immediately following the ones that were erased. */
	iterator erase(	iterator itFirst,				///< [in] Iterator to first element to remove
					iterator itLast)				///< [in] Iterator to one past last element to remove. 
	{
		if ( empty()) return iterator(0);
		node_pointer pBeforeFirst	= itFirst.get_node_pointer();
		node_pointer pLast			= itLast.get_node_pointer();
		if ( pBeforeFirst == m_pFirst )
		{	// First to delete is front of list
			pLast		= erase_range(pBeforeFirst, pLast);
			pop_front();			// Delete front element
			m_pFirst	= pLast;	// m_pFirst must point to pLast after this function ...
			return iterator(pLast);
		}
		else
		{
			pBeforeFirst = find_previous(pBeforeFirst);
			return iterator( erase_range(pBeforeFirst, pLast) );
		}
	}

	/// Erase all elements of the container.
	void clear() 
	{
		erase(begin(), end());	
	}



	// Remove element. Erase each element matching elem
	void remove(const_reference elem)				///< [in] Element to erase each instance of.
	{	
		iterator itLast = end();
		for (iterator itFirst = begin(); itFirst != itLast; )
		{
			if	(*itFirst == elem)	itFirst = erase(itFirst);
			else					++itFirst;
		}
	}


	// --------------------
	// --- Info methods ---
	// --------------------

	/**	Why is "size()" linear in time? 
		Because STL standard only promises O(n) time
		in the size() function of containers. Which in turn is to save containers from
		updating an internal m_iSize variable every time objects are inserted or removed,
		which would use a lot of time in those frequent operations. On the other hand, 
		getting the size of the contaner is not a very frequent operation.*/
	size_type		size() const 
	{
		size_type i= 0;
		const_iterator it= begin();
		while(it != end()) 
		{
			++i;
			++it;
		}
		return i;
	}
	
	/// Test for empty container.
	bool			empty() const	
	{
		return m_pFirst == 0;
	}

	/** Maximum container size. 
		IMPORTANT: Allways use the allocaters max_size() when calculating max_size().
		\return maximum possible length of container. 
	*/
	size_type		max_size() const
	{			
		return m_NodeAlloc.max_size();
	}


private:
	// -------------------------------------------
	// --- Create/destroy node methods PRIVATE ---
	// -------------------------------------------
	
	/** Create node.
		We only allocate space for the node, but don't call the constructor for 
		'elem'. At least it seems like that's the way STL container classes do it!*/
	node_pointer	create_node(const_reference elem)
	{
		node_pointer pNode = m_NodeAlloc.allocate(1);	// Allocate space for 1 node
		m_NodeAlloc.construct(pNode, node_value_type(elem));
		return pNode;

	//	The approach (commented out below )does not work when container type T is 
	//	const specified like in the declaration: my_container<const int> Con;
	//	Here the pNode->m_Data becomes const and therefore it can only be assigned
	//	via the constructor for my_container_node (= node_value_type).
	//	Alternatively one might "cheat" a bit and use the (simple) loki template 
	//	TypeTraits<T>::NonConstType to strip the const specification of m_Data.
	//	This approach might save a constructor call of my_container_node. 
//		pNode->m_Data	= elem;
//		pNode->m_pNext	= 0;
//		return pNode;
	}

	// ------------------------------
	// --- Helper methods PRIVATE ---
	// ------------------------------

	/** Delete node.*/
	void	delete_node(node_pointer pNode)					///< [in] Pointer to node to delete
	{
		m_NodeAlloc.destroy(pNode);
//		m_NodeAlloc.destroy(pNode->m_Data);	// CHECK: Does this work? Possible faster than the line above
		m_NodeAlloc.deallocate(pNode, 1);
	}

	/** Erase node after the given.
		\return A pointer to the element immediately following the one that was erased. */
	node_pointer erase_after(node_pointer pNode)			///< [in] Node after this is erased from list
	{
		node_pointer	pDel		= pNode->m_pNext;
		node_pointer	pDelNext	= pDel->m_pNext;
		pNode->m_pNext				= pDelNext;
		if ( pDel == m_pLast ) m_pLast = pNode;
		delete_node(pDel);
		return pDelNext;
	}

	/** Erases the range [pBeforeFirst->m_pNext, pLast). Only call this function 
		if list has a least two elements.
		\return  A pointer to the element immediately following the ones that were 
		erased. */
	node_pointer erase_range(node_pointer pBeforeFirst,		///< [in] Pointer to element before the one to remove
							 node_pointer pLast)			///< [in] Pointer to one past last element to remove
	{
		node_pointer pNode = pBeforeFirst->m_pNext;
		while ( pNode != pLast )
		{
			node_pointer pDel	= pNode;
			pNode				= pNode->m_pNext;
			// If removing the last node in range, then m_pLast must be corrected to
			// point to pBeforeFirst. NOTE: This if inside the loop is not speed
			// optimal, but since this class is mainly to show iterators/allocators, 
			// we'll just leave it like this!
			if ( pDel == m_pLast ) 
			{
				m_pLast = pBeforeFirst; 
				m_pLast->m_pNext = 0;
			}
			delete_node(pDel);	// Delete the node
		}
		pBeforeFirst->m_pNext	= pLast;
		return pLast;
	}


	/** Finds the pointer to the node previous to the element. Don't call this 
		function if pNode == m_pFirst */
	node_pointer find_previous(	const_reference elem,		///< [in] Element to look for.
								node_pointer pNode) const	///< [in] Node to start from.
	{
		while ( pNode->m_pNext )
		{
			if ( pNode->m_pNext->m_Data == elem ) return pNode;
			pNode = pNode->m_pNext;
		}
		return 0; // Return null pointer if we didn't find anything
	}

	/** Finds the pointer to the node previous to pointer given. Don't call this 
		function if pNode == m_pFirst */
	node_pointer find_previous(	node_pointer pNode) const	///< [in] Node pointer to look for.
	{
		node_pointer pPrev = m_pFirst;
		while (pPrev->m_pNext != pNode) pPrev = pPrev->m_pNext;
		return pPrev;
	}


	// --------------------------------
	// --- Member variables PRIVATE ---
	// --------------------------------
	node_allocator		m_NodeAlloc;	
	node_pointer		m_pFirst;			///< Points to FIRST node in linked list
	node_pointer		m_pLast;			///< Points to LAST actual node in linked list (not to one past the last)
};




_STL_BOOST_END_NAMESPACE


#endif

// #########################################################################
// ### OLD VERSION OF THE ITERATORS WHICH DOES NOT USE 'select' TEMPLATE ###
// #########################################################################

/*
	// --------------------------------------
	// --- Iterators Inner classes PUBLIC ---
	// --------------------------------------
public:

	template <typename REFERENCE, typename NODE_POINTER, typename POINTER> 
	class iterator_base
	{
		friend class iterator_base;
	public:
		typedef typename	std::forward_iterator_tag	iterator_category;	///< Iterator category. Certain algorithms need certain kinds of iterators.
		typedef typename	A::value_type				value_type;			///< The type of object, T, pointed to by the iterator.
		typedef typename	POINTER						pointer;			///< Pointer to T.			
		typedef typename	A::difference_type			difference_type;	///< CHECK: Not needed unless for random iterators.
		typedef typename	REFERENCE					reference;			///< Reference to T. Can be either const or not depending on REFERENCE
		typedef typename	NODE_POINTER				node_pointer;		///< Reference to T. Can be either const or not depending on REFERENCE

		iterator_base() : m_pNode(0) { }

		iterator_base(node_pointer pNode)					///< [in] Pointer to my_container_node.
			: m_pNode(pNode) { }

		// Copy constructor template. Construct from general 
		// iterator_base<REFERENCE, NODE_POINTER, POINTER> 
		template <typename R, typename NP, typename P> 
		iterator_base(const iterator_base<R, NP, P>& it)	///< [in] Iterator to construct from.
			: m_pNode(it.m_pNode) { }


		reference			operator*() const 
		{
			return m_pNode->m_Data;	// Return designated value.
		}

		pointer				operator->() const 
		{	// Delegate to operator '*'
			return (&**this);	// Return pointer to class object
		}

		iterator_base&		operator++()
		{
			m_pNode = m_pNode->m_pNext;		// Increment
			return *this;					// Return incremented iterator
		}

		const iterator_base	operator++(int)
		{
			iterator_base itTemp(*this);	// Make copy of original
			m_pNode = m_pNode->m_pNext;		// Increment
			return itTemp;					// Return copy of original
		}


		//	Equality test of iterators. Since the iterator_base class is a template 
		//	we make this operator (and possible other BINARY operators) a function
		//	template. This is because different instantiations of this class leads 
		//	to distinct types and we want '==' to work between ALL of them. 
		//	But instead of writing 4 versions of '==', which all does essentially
		//	the same we take the lazy approach an let the compiler do the work.
		//	The 4 '!=' is conviniently handled by the:
		//	"using tutils::rel_ops::operator!=;" statement in the top of this file.
		//	\see the 'tutils/rel_ops.hpp' file for more explanation. 
		template <typename R2, typename NP2, typename P> 
		bool				operator==(	const iterator_base<R2, NP2, P>& itR)	const ///< [in] Right operand.    
		{
			return ( this->m_pNode == itR.m_pNode );		// Compare pointers
		}

	private:
		//	The type of m_pNode is either 'node_pointer' or 'node_const_pointer'
		//	depending on wheter this class is typedef'ed as (mutable) iterator (pointee
		//	object can BE modified)	or (non mutable) const_iterator (pointee object
		//	can NOT be modified).
		//	This ensures fx. that one can assign a mutable 'iterator' to an non mutable 
		//	const_itarator, but NOT vice versa. Ensured be the fact that you can assign
		//	a 'pObj*' to a 'const pObj*' but NOT the other way round! 
		node_pointer		m_pNode;	 
	};

	
	// --- ITERATOR TYPEDEFS ---

	// Iterator. Used to iterate through a container. This is the normal mutable 
	// iterator which allows modifying the pointed to object T.
	typedef iterator_base<reference, node_pointer, pointer> iterator;


	// Const iterator. Used to iterate through a container. Not able to modify 
	// the pointed to object T. 
	typedef iterator_base<const_reference, node_const_pointer, const_pointer> const_iterator;


*/


