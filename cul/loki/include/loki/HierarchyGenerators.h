////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////
#ifndef LOKI_HIERARCHYGENERATORS_INC_
#define LOKI_HIERARCHYGENERATORS_INC_

// $Id: HierarchyGenerators.h 751 2006-10-17 19:50:37Z syntheticpp $


#include "Typelist.h"
#include "TypeTraits.h"
#include "EmptyType.h"

namespace Loki
{
#if defined(_MSC_VER) && _MSC_VER >= 1300
#pragma warning( push ) 
 // 'class1' : base-class 'class2' is already a base-class of 'class3'
#pragma warning( disable : 4584 )
#endif // _MSC_VER

////////////////////////////////////////////////////////////////////////////////
// class template GenScatterHierarchy
// Generates a scattered hierarchy starting from a typelist and a template
// Invocation (TList is a typelist, Unit is a template of one arg):
// GenScatterHierarchy<TList, Unit>
// The generated class inherits all classes generated by instantiating the 
// template 'Unit' with the types contained in TList 
////////////////////////////////////////////////////////////////////////////////

    namespace Private
    {
        // The following type helps to overcome subtle flaw in the original 
        // implementation of GenScatterHierarchy. 
        // The flaw is revealed when the input type list of GenScatterHierarchy 
        // contains more then one element of the same type (e.g. LOKI_TYPELIST_2(int, int)). 
        // In this case GenScatterHierarchy will contain multiple bases of the same 
        // type and some of them will not be reachable (per 10.3).
        // For example before the fix the first element of Tuple<LOKI_TYPELIST_2(int, int)>
        // is not reachable in any way!
        template<class, class> 
        struct ScatterHierarchyTag;
    }

    template <class TList, template <class> class Unit>
    class GenScatterHierarchy;
     
    template <class T1, class T2, template <class> class Unit>
    class GenScatterHierarchy<Typelist<T1, T2>, Unit>
        : public GenScatterHierarchy<Private::ScatterHierarchyTag<T1, T2>, Unit>
        , public GenScatterHierarchy<T2, Unit>
    {
    public:
        typedef Typelist<T1, T2> TList;
        // Insure that LeftBase is unique and therefore reachable
        typedef GenScatterHierarchy<Private::ScatterHierarchyTag<T1, T2>, Unit> LeftBase;
        typedef GenScatterHierarchy<T2, Unit> RightBase;
        template <typename T> struct Rebind
        {
            typedef Unit<T> Result;
        };
    };
     
    // In the middle *unique* class that resolve possible ambiguity
    template <class T1, class T2, template <class> class Unit>
    class GenScatterHierarchy<Private::ScatterHierarchyTag<T1, T2>, Unit> 
        : public GenScatterHierarchy<T1, Unit>
    {
    };

    template <class AtomicType, template <class> class Unit>
    class GenScatterHierarchy : public Unit<AtomicType>
    {
        typedef Unit<AtomicType> LeftBase;
        template <typename T> struct Rebind
        {
            typedef Unit<T> Result;
        };
    };
    
    template <template <class> class Unit>
    class GenScatterHierarchy<NullType, Unit>
    {
        template <typename T> struct Rebind
        {
            typedef Unit<T> Result;
        };
    };
     
////////////////////////////////////////////////////////////////////////////////
// function template Field
// Accesses a field in an object of a type generated with GenScatterHierarchy
// Invocation (obj is an object of a type H generated with GenScatterHierarchy,
//     T is a type in the typelist used to generate H):
// Field<T>(obj)
// returns a reference to Unit<T>, where Unit is the template used to generate H 
////////////////////////////////////////////////////////////////////////////////

    template <class T, class H>
    typename H::template Rebind<T>::Result& Field(H& obj)
    {
        return obj;
    }
     
    template <class T, class H>
    const typename H::template Rebind<T>::Result& Field(const H& obj)
    {
        return obj;
    }
     
////////////////////////////////////////////////////////////////////////////////
// function template TupleUnit
// The building block of tuples 
////////////////////////////////////////////////////////////////////////////////

    template <class T>
    struct TupleUnit
    {
        T value_;
        operator T&() { return value_; }
        operator const T&() const { return value_; }
    };

////////////////////////////////////////////////////////////////////////////////
// class template Tuple
// Implements a tuple class that holds a number of values and provides field 
//     access to them via the Field function (below) 
////////////////////////////////////////////////////////////////////////////////

    template <class TList>
    struct Tuple : public GenScatterHierarchy<TList, TupleUnit>
    {
    };

////////////////////////////////////////////////////////////////////////////////
// helper class template FieldHelper
// See Field below
////////////////////////////////////////////////////////////////////////////////

    template <class H, unsigned int i> struct FieldHelper;
    
    template <class H>
    struct FieldHelper<H, 0>
    {
        typedef typename H::TList::Head ElementType;
        typedef typename H::template Rebind<ElementType>::Result UnitType;
        
        enum
        {
            isTuple = Conversion<UnitType, TupleUnit<ElementType> >::sameType,
            isConst = TypeTraits<H>::isConst
        };

        typedef const typename H::LeftBase ConstLeftBase;
        
        typedef typename Select<isConst, ConstLeftBase, 
            typename H::LeftBase>::Result LeftBase;
            
        typedef typename Select<isTuple, ElementType, 
            UnitType>::Result UnqualifiedResultType;

        typedef typename Select<isConst, const UnqualifiedResultType,
                        UnqualifiedResultType>::Result ResultType;
            
        static ResultType& Do(H& obj)
        {
            LeftBase& leftBase = obj;
            return leftBase;
        }
    };

    template <class H, unsigned int i>
    struct FieldHelper
    {
        typedef typename TL::TypeAt<typename H::TList, i>::Result ElementType;
        typedef typename H::template Rebind<ElementType>::Result UnitType;
        
        enum
        {
            isTuple = Conversion<UnitType, TupleUnit<ElementType> >::sameType,
            isConst = TypeTraits<H>::isConst
        };

        typedef const typename H::RightBase ConstRightBase;
        
        typedef typename Select<isConst, ConstRightBase, 
            typename H::RightBase>::Result RightBase;

        typedef typename Select<isTuple, ElementType, 
            UnitType>::Result UnqualifiedResultType;

        typedef typename Select<isConst, const UnqualifiedResultType,
                        UnqualifiedResultType>::Result ResultType;
            
        static ResultType& Do(H& obj)
        {
            RightBase& rightBase = obj;
            return FieldHelper<RightBase, i - 1>::Do(rightBase);
        }
    };

////////////////////////////////////////////////////////////////////////////////
// function template Field
// Accesses a field in an object of a type generated with GenScatterHierarchy
// Invocation (obj is an object of a type H generated with GenScatterHierarchy,
//     i is the index of a type in the typelist used to generate H):
// Field<i>(obj)
// returns a reference to Unit<T>, where Unit is the template used to generate H
//     and T is the i-th type in the typelist 
////////////////////////////////////////////////////////////////////////////////

    template <unsigned int i, class H>
    typename FieldHelper<H, i>::ResultType&
    Field(H& obj)
    {
        return FieldHelper<H, i>::Do(obj);
    }
        
//    template <int i, class H>
//    const typename FieldHelper<H, i>::ResultType&
//    Field(const H& obj)
//    {
//        return FieldHelper<H, i>::Do(obj);
//    }
        
////////////////////////////////////////////////////////////////////////////////
// class template GenLinearHierarchy
// Generates a linear hierarchy starting from a typelist and a template
// Invocation (TList is a typelist, Unit is a template of two args):
// GenScatterHierarchy<TList, Unit>
////////////////////////////////////////////////////////////////////////////////

    template
    <
        class TList,
        template <class AtomicType, class Base> class Unit,
        class Root = EmptyType
    >
    class GenLinearHierarchy;
    
    template
    <
        class T1,
        class T2,
        template <class, class> class Unit,
        class Root
    >
    class GenLinearHierarchy<Typelist<T1, T2>, Unit, Root>
        : public Unit< T1, GenLinearHierarchy<T2, Unit, Root> >
    {
    };

    template
    <
        class T,
        template <class, class> class Unit,
        class Root
    >
    class GenLinearHierarchy<Typelist<T, NullType>, Unit, Root>
        : public Unit<T, Root>
    {
    };

    template
    <
        template <class, class> class Unit,
        class Root
    >
    class GenLinearHierarchy<NullType , Unit, Root>
        : public Root // is this better: Unit<NullType, Root> ?
    {
    };

#if defined(_MSC_VER) && _MSC_VER >= 1300
#pragma warning( pop ) 
#endif
}   // namespace Loki

#endif // end file guardian

