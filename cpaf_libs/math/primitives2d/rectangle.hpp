#ifndef _TMATH_P2D_RECTANGLE_
#define _TMATH_P2D_RECTANGLE_
// ****************************************************************************
// CREATED_ON	: 27-09-05
// CREATED_BY	: Martin Lutken
// ----------------------------------------------------------------------------
// COPYRIGHT	:
// ****************************************************************************


#include <math/base/v2.hpp>
#include <math/base/g33.hpp>


_P2D_BEGIN_NAMESPACE

/// DOXYS_OFF

/** Rectangle store by value policy. */
template <typename T>
class PRectangleStoreByValue
{
public:

    typedef v2<T>					v2_storage_type;

    // --- vector typedefs ---
    typedef v2<T>					v2_value_type;
    typedef v2_value_type&			v2_reference;
    typedef const v2_value_type&	v2_const_reference;

    /// Default constructor with no initialization.
    PRectangleStoreByValue() {}


    /// Copy constructor.
    PRectangleStoreByValue(const PRectangleStoreByValue& t)
        : 	m_vUL(t.m_vUL), m_vLR(t.m_vLR) {}


    /// Constructor with 2 points initialization.
    PRectangleStoreByValue(	v2_storage_type vUL,	///< [in] Rectangle upper left corner of rectangle
                            v2_storage_type vLR)	///< [in] Rectangle lower right corner
        : 	m_vUL(vUL), m_vLR(vLR)  {}

    /// Constructor with 4 values initialization.
    PRectangleStoreByValue(	T ULx,	///< [in] Rectangle upper left corner X
                            T ULy,	///< [in] Rectangle upper left corner Y
                            T LRx,	///< [in] Rectangle lower right corner X
                            T LRy)	///< [in] Rectangle lower right corner Y
        : 	m_vUL(ULx, ULy), m_vLR(LRx, LRy)  {}

    // --- Access elements ---

    /// Access upper left corner of rectangle. \return Reference to upper left corner of rectangle.
    v2_reference		ul()			{ return m_vUL; }
    v2_const_reference	ul()	const	{ return m_vUL; }

    /// Access lower right corner of rectangle. \return Reference to lower right corner of rectangle.
    v2_reference		lr()			{ return m_vLR; }
    v2_const_reference	lr()	const	{ return m_vLR; }


private:
    v2_storage_type			m_vUL;		///< Rectangle upper left corner of rectangle.
    v2_storage_type			m_vLR;		///< Rectangle lower right corner of rectangle, same as width , height)
};



/** Rectangle store by pointer policy. */
template <typename T>
class PRectangleStoreByPointer
{
public:

    typedef v2<T>*					v2_storage_type;

    // --- vector typedefs ---
    typedef v2<T>					v2_value_type;
    typedef v2_value_type&			v2_reference;
    typedef const v2_value_type&	v2_const_reference;

    /// Default constructor with no initialization. \important NOT supported !!!
    PRectangleStoreByPointer() {
        STATIC_CHECK(0, class_rectangle_using_PRectangleStoreByPointer_policy_does_NOT_support_default_constructor);
    }

    /// Copy constructor. \important NOT supported !!!
    PRectangleStoreByPointer(const PRectangleStoreByPointer& t) {
        STATIC_CHECK(0, class_rectangle_using_PRectangleStoreByPointer_policy_does_NOT_support_copy_constructor);
    }

    /// Constructor with 2 points initialization.
    PRectangleStoreByPointer(v2_storage_type pvUL,		///< [in] Pointer to rectangle upper left corner
                            v2_storage_type pvLR)		///< [in] Pointer to rectangle lower right corner
        : 	m_pvUL(pvUL), m_pvLR(pvLR) {}

    /// Constructor with 4 values initialization. \important NOT supported !!!
    PRectangleStoreByPointer(	T ULx, T ULy, T LRx, T LRy)	{
        STATIC_CHECK(0, class_rectangle_using_PRectangleStoreByPointer_policy_does_NOT_support_4_values_constructor);
    }

    // --- Access elements ---

    /// Access upper left corner of rectangle. \return Reference to upper left corner of rectangle.
    v2_reference		ul()			{ return *m_pvUL; }
    v2_const_reference	ul()	const	{ return *m_pvUL; }

    /// Access lower right corner of rectangle. \return Reference to lower right corner of rectangle.
    v2_reference		lr()			{ return *m_pvLR; }
    v2_const_reference	lr()	const	{ return *m_pvLR; }


private:
    v2_storage_type			m_pvUL;		///< Pointer to rectangle upper left corner
    v2_storage_type			m_pvLR;		///< Pointer to rectangle lower right corner
};



/** Rectangle store by reference policy. */
template <typename T>
class PRectangleStoreByReference
{
public:

    typedef v2<T>&					v2_storage_type;

    // --- vector typedefs ---
    typedef v2<T>					v2_value_type;
    typedef v2_value_type&			v2_reference;
    typedef const v2_value_type&	v2_const_reference;

    /// Default constructor with no initialization. \important NOT supported !!!
    PRectangleStoreByReference() {
        STATIC_CHECK(0, class_rectangle_using_PRectangleStoreByReference_policy_does_NOT_support_default_constructor);
    }

    /// Copy constructor. \important NOT supported !!!
    PRectangleStoreByReference(const PRectangleStoreByReference& t) {
        STATIC_CHECK(0, class_rectangle_using_PRectangleStoreByReference_policy_does_NOT_support_copy_constructor);
    }

    /// Constructor with 2 points initialization.
    PRectangleStoreByReference(v2_storage_type rvUL,	///< [in] Reference to rectangle upper left corner
                               v2_storage_type rvLR)	///< [in] Reference to rectangle lower right corner
        : 	m_rvUL(rvUL), m_rvLR(rvLR)  {}

    /// Constructor with 4 values initialization. \important NOT supported !!!
    PRectangleStoreByReference( T ULx, T ULy, T LRx, T LRy ) {
        STATIC_CHECK(0, class_rectangle_using_PRectangleStoreByReference_policy_does_NOT_support_4_values_constructor);
    }
    // --- Access elements ---

    /// Access upper left corner of rectangle. \return Reference to upper left corner of rectangle.
    v2_reference		ul()			{ return m_rvUL; }
    v2_const_reference	ul()	const	{ return m_rvUL; }

    /// Access lower right corner of rectangle. \return Reference to lower right corner of rectangle.
    v2_reference		lr()			{ return m_rvLR; }
    v2_const_reference	lr()	const	{ return m_rvLR; }


private:
    v2_storage_type			m_rvUL;		///< Reference to rectangle upper left corner
    v2_storage_type			m_rvLR;		///< Reference to rectangle lower right corner
};

/// DOXYS_ON

/**
Rectangle (2D) defined by two points. An upper-left corner
( rectangle::ul() ) and a lower-right corner (rectangle::lr()).  */
template
<
    typename T,
    template <class> class StoragePolicy = PRectangleStoreByValue
>
class rectangle
{
    // ----------------
    // --- Typedefs ---
    // ----------------

public:
    typedef rectangle<T, StoragePolicy >	value_type;
    typedef value_type&						reference;
    typedef const value_type&				const_reference;
    typedef value_type*						pointer;
    typedef const value_type*				const_pointer;

    /// For operators and functions returning an object of type rectangle.
    /// Since we get temporary object created this way we need these temporarys
    /// to have it's own store for the whole rectangle.
    typedef rectangle<T, PRectangleStoreByValue>	value_value_type;

    // --- v2 storage type typedefs ---
    typedef typename StoragePolicy<T>::v2_storage_type  v2_storage_type;

    // --- vector typedefs ---
    typedef v2<T>					v2_value_type;
    typedef v2_value_type&			v2_reference;
    typedef const v2_value_type&	v2_const_reference;

    // --- m22 typedefs ---
    typedef m22<T>					m22_value_type;
    typedef m22_value_type&			m22_reference;
    typedef const m22_value_type&	m22_const_reference;

    // --- g33 typedefs ---
    typedef g33<T>					g33_value_type;
    typedef g33_value_type&			g33_reference;
    typedef const g33_value_type&	g33_const_reference;


    // --------------------------------------------
    // --- Constructors / Set values functions. ---
    // --------------------------------------------

    /// Default constructor with no initialization.
    rectangle() : m_Data() {}


    /// Copy constructor. Initializing with another rectangle.
    rectangle(const_reference t) : m_Data(t.m_Data) {}

    /** Constructor with 2 points initialization. */
    rectangle(	v2_storage_type vUL,		///< [in] Rectangle upper left corner
                v2_storage_type vLR)		///< [in] Rectangle lower right corner
        : 	m_Data(vUL, vLR) {}

    /// Constructor with 4 values initialization.
    rectangle(	T ULx,	///< [in] Rectangle upper left corner X
                T ULy,	///< [in] Rectangle upper left corner Y
                T LRx,	///< [in] Rectangle lower right corner X
                T LRy)	///< [in] Rectangle lower right corner Y
        : 	m_Data(ULx, ULy, LRx, LRy)  {}

    /// Cast to rectangle with different number-type 'T'.
    template <typename T1>
    operator rectangle<T1>() const
    {
        typedef rectangle<T1> rectangle_value_type;
        return rectangle_value_type( ul(), lr() );
    }


    // ------------------------
    // --- Access functions ---
    // ------------------------

    /// Access upper left corner of rectangle. \return Reference to upper left corner of rectangle.
    v2_reference		ul()			{ return m_Data.ul(); }
    v2_const_reference	ul()	const	{ return m_Data.ul(); }

    /// Access lower right corner of rectangle. \return Reference to lower right corner of rectangle.
    v2_reference		lr()			{ return m_Data.lr(); }
    v2_const_reference	lr()	const	{ return m_Data.lr(); }

//	/// Access width of rectangle. This is the same as rectangle::lr().x().
//	/// \return Reference to width of rectangle (lower right corner x-coordinate).
// 	T&					w()			{ return m_Data.lr().x(); }
// 	T					w()	const	{ return m_Data.lr().x(); }

//	/// Access height of rectangle. This is the same as rectangle::lr().y().
//	/// \return Reference to height of rectangle (lower right corner y-coordinate).
// 	T&					h()			{ return m_Data.lr().y(); }
// 	T					h()	const	{ return m_Data.lr().y(); }

    /// Access x-coordinate of rectangle. This is the same as rectangle::ul().x().
    /// \return Reference to x-coordinate of rectangle (upper left corner x-coordinate).
    T&					x()			{ return m_Data.ul().x(); }
    T					x()	const	{ return m_Data.ul().x(); }

    /// Access y-coordinate of rectangle. This is the same as rectangle::ul().y().
    /// \return Reference to x-coordinate of rectangle (upper left corner y-coordinate).
    T&					y()			{ return m_Data.ul().y(); }
    T					y()	const	{ return m_Data.ul().y(); }

    // ----------------------------
    // --- Set values functions ---
    // ----------------------------

    /** Set rectangle with  points. */
    void		set(v2_const_reference vUL,		///< [in] Rectangle upper left corner
                    v2_const_reference vLR)		///< [in] Rectangle lower right corner
    {
        ul() = vUL;
        lr() = vLR;
    }


    /** Set rectangle with 4 values. */
    void		set(T ULx,			///< [in] Rectangle upper left corner, X-coordinate.
                    T ULy,			///< [in] Rectangle upper left corner, Y-coordinate.
                    T LRx,			///< [in] Rectangle lower right corner, X-coordinate.
                    T LRy)			///< [in] Rectangle lower right corner, Y-coordinate.
    {
        ul().set(ULx, ULy);
        lr().set(LRx, LRy);
    }



    // ----------------------------
    // --- Assignment operators ---
    // ----------------------------

    /** '=' operator, Assignment from another rectangle.
        \note Here we allways copy values to destination. In case this rectangle does not
              use the PRectangleStoreByValue - then it is still values that are copied - not
              pointers or references, that are assigned. */
    template < template <class> class StorPolicy >
    reference			operator =(const rectangle<T, StorPolicy>& t)		///< [in] Right hand side operand.
    {
        ul() = t.ul();
        lr() = t.lr();
        return *this;
    }

    // ------------------------------------
    // --- Add/Substract math operators ---
    // ------------------------------------

    /** '+=' operator, Add vector to rectangle. Translate rectangle to other position.
        \return Reference to "this".*/
    reference			operator +=(v2_value_type v)		///< [in] Vector to add to all points.
    {
        ul() += v;
        lr() += v;
        return *this;
    }


    /** '-=' operator, Subtract vector from rectangle. Translate rectangle to other position.
        \return Reference to "this".*/
    reference			operator -=(v2_value_type v)		///< [in] Vector to subtract to all points.
    {
        ul() -= v;
        lr() -= v;
        return *this;
    }

    /** '+' operator, Add vector to rectangle. Translate rectangle to other position.
        \return rectangle translated to the new position. */
    value_value_type	operator +(	v2_const_reference v		///< [in] Vector to add to all points.
                              )const
    {
        return value_value_type( ul() + v, lr() + v);
    }

    /** '-' operator, Subtract vector from rectangle. Translate rectangle to other position.
        \return rectangle translated to the new position. */
    value_value_type	operator -(	v2_const_reference v		///< [in] Vector to subtract to all points.
                              )const
    {
        return value_value_type( ul() - v, lr() - v);
    }


    // -------------------------------------
    // --- Multiplication math operators ---
    // -------------------------------------

    /** '*=' operator, Scales the rectangle with a scalar. (I.e. multiplying the
        points with a scalar.)
        \return Reference to "this".*/
    reference			operator *=(T Val)		///< [in] Right hand side operand. Scalar used to scale the vector.
    {
        ul() *= Val;
        lr() *= Val;
        return *this;
    }

    /** '*' operator, Rectangle - Scalar multiplication.
        Scales rectangle with a scalar, like this: matrix * number.
        \return Rectangle holding the result of the scaling. */
    value_value_type	operator *(T Val			///< [in] Right hand side operand scalar (T).
                                  )const
    {
        return value_value_type( ul()*Val, lr()*Val );
    }

    /** '*' operator. Rectangle - Matrix (m22) multiplication. rectangle * m22. Typically
            used to rotate the rectangle. */
    value_value_type	operator *(m22_const_reference M22 	///< [in] Right hand side operand (m22).
                              ) const
    {
        return value_value_type( ul()*M22, lr()*M22 );
    }

    /** '*' operator. Rectangle - Matrix (g33) multiplication. rectangle * g33. Typically
            used to transform the rectangle. */
    value_value_type	operator *(g33_const_reference G33 	///< [in] Right hand side operand (g33).
                              ) const
    {
        return value_value_type( ul()*G33, lr()*G33 );
    }


    // ----------------------
    // --- Util functions ---
    // ----------------------

    /// Access width of rectangle. \return Width of rectangle.
    T					w()	const	{ return Abs(ul().x() - lr().x()); }

    /// Get height of rectangle. \return Height of rectangle.
    T					h()	const	{ return Abs(ul().y() - lr().y()); }

    /// Get lower left corner of rectangle. \return Copy (2D vector as value type) of lower left corner of rectangle.
    v2_value_type		ll()		{ return v2_value_type( ul().x(), lr().y() ); }

    /// Get upper right corner of rectangle. \return Copy (2D vector as value type) of upper right corner of rectangle.
    v2_value_type		ur()		{ return v2_value_type( lr().x(), ul().y() ); }

    /** Get vector from rectangle upper left corner to lower right corner (diagonal) \return Vector from upper left corner to lower right corner. */
    v2_value_type		vec_ul_lr() const 	{ return lr() - ul();}

    /** Get vector from rectangle lower right corner to upper left corner (diagonal) \return Vector from lower right corner to upper left corner. */
    v2_value_type		vec_lr_ul() const 	{ return ul() - lr();}

    /** Get center of rectangle \return Center of rectangle. */
    v2_value_type		center() const 		{ return (ul() + lr()) / T(2) ;}


    /** Get area of rectangle.
        \return Area of rectangle, which is allways positive or zero. */
    T				area() const
    {
        return w()*h();
    }

    /** Test if point is inside rectangle.
    \return True if point given is inside the rectangle. */
    bool_t			point_inside(v2_const_reference p0	///< [in] Point to test
                                ) const
    {
        v2_value_type p0T = p0 - center();	// Simulate that rectangle has center in origo.
//		return (Abs(p0T.x()) <= (w() / T(2)) ) && (Abs(p0T.y()) <= (h() / T(2)) );
        return (Abs(T(2)*p0T.x()) <= w() ) && (Abs(T(2)*p0T.y()) <= h() );
    }

    // -------------------------
    // --- Compare operators ---
    // -------------------------

    /** '==' operator. Tests two rectangles for equality.
        EPSILON<T>() is used as threshold to avoid numeric instabilities. Use
        the 'equal_exact' member function if exact comparision is required.
        \return True if all corresponding elements of the two rectangles is no further,
        than EPSILON<T>() apart (absolute value), false otherwise. */
    bool_t	operator ==(const_reference r			///< [in] Right operand.
                       ) const
    {
        return	(ul() == r.ul()) &&
                (lr() == r.lr());
    }

    /** '!=' operator. Tests two rectangles for in-equality.
        EPSILON<T>() is used as threshold to avoid numeric instabilities. Use
        the 'equal_exact' member function if exact comparision is required.
        \return True if just one corresponding pair of elements of the two
        rectangles is further than EPSILON<T>() apart (absolute value),
        false otherwise. */
    bool_t	operator !=(const_reference r			///< [in] Right operand.
                       ) const
    {
        return	(ul() != r.ul()) ||
                (lr() != r.lr());
    }


    // -------------------------
    // --- Compare functions ---
    // -------------------------


    /** Equal with exact comparision. Tests two rectangles for equality.
        No epsilon value is used. Use with care when comparing floating point
        type values with this function.
        \return True if all corresponding elements of the two rectangles are exactly
        equal, false otherwise. */
    bool_t		equal_exact(const_reference r)		///< [in] Rectangle to compare with
    {
        return	ul().equal_exact(r.ul()) &&
                lr().equal_exact(r.lr());
    }


    /** Equal with custom epsilon comparision. Tests two vectors for equality.
        Supplied epsilon value is used.
        \return True if all corresponding elements of the two vectors is no further,
        than epsilon apart (absolute value), false otherwise.. */
    bool_t		equal_epsilon(const_reference r,	///< [in] Rectangle to compare with
                              T epsilon				///< [in] Epsilon value to use when comparing
                             ) const
    {
        return	ul().equal_epsilon(r.ul(), epsilon) &&
                lr().equal_epsilon(r.lr(), epsilon);
    }


    // -----------------------
    // --- Debug Functions ---
    // -----------------------

    /// Convert rectangle to a string for debug print.
    std::string			str(int_t iDecimals= -1	///< [in] Number of decimals
                           ) const
    {
        std::string s = "UL: " + ul().str(iDecimals);
                    s += "\nLR: " + lr().str(iDecimals);
        return s;
    }

private:
    // ------------
    // --- Data ---
    // ------------
    StoragePolicy<T>			m_Data;

};


// -----------------
// --- Operators ---
// -----------------

// '*' operator, Scalar - rectangle multiplication. Scales the rectangle with a scalar.
//template < typename T, template <class> class StoragePolicy >
template < typename T, template <class> class StoragePolicy >
inline rectangle<T, PRectangleStoreByValue>
    operator *(	T Val,		 							// [in] Left Hand Side operand (scalar).
                const rectangle<T, StoragePolicy>& t)	// [in] Right hand side operand rectangle.
{
    return t*Val;	// Implement using operator rectangle * T
}



// ------------------------
// --- Stream operators ---
// ------------------------

/** '<<' operator, Write rectangle to stream.
    \return Reference to (modified) ostream. */
template < typename T, template <class> class StoragePolicy >
inline std::ostream&
    operator <<(std::ostream& os,						///< [out] Output stream to write to.
                const rectangle<T, StoragePolicy>& r)	///< [in] Rectangle to write.
{
    os<<r.ul()<<r.lr();
    return os;
}

/** '>>' operator. Read rectangle from input stream. */
template < typename T, template <class> class StoragePolicy >
inline std::istream&
    operator >>(std::istream& is,				///< [in]  Input stream to read from.
                rectangle<T, StoragePolicy>& r)	///< [out] Rectangle recieve the data from the stream.
{
    is>>r.ul()>>r.lr();
    return is;
}

namespace i32 {
    using rectangle = cpaf::math::p2d::rectangle<int32_t>;
}

_P2D_END_NAMESPACE



#endif	// _TMATH_P2D_RECTANGLE_
