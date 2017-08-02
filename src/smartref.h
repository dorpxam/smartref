#pragma once

#include "reflection.h"

#include <type_traits>

namespace smartref {

template<typename Delegate, class Derived = void>
class using_;

template<template<typename, typename> class Class, typename Delegate, class Derived>
auto DerivedType(Class<Delegate, Derived> &) -> Derived;

template<template<typename, typename> class Class, typename Delegate, class Derived>
auto DelegateType(Class<Delegate, Derived> &) -> Delegate;

template<class Base>
auto &delegate(Base &base)
{
    using Derived = decltype(DerivedType(base));
    static_assert(std::is_base_of<Base, Derived>::value);

    //! Downcast to the derived class
    auto &derived = static_cast<Derived &>(base);

    using Delegate = decltype(DelegateType(base));

    //! Now invoke the conversion operator
    return static_cast<Delegate &>(derived);
}

template<class Class, typename... T>
struct DelayedImpl
{
    using type = Class;
};

template<class Class, typename... T>
using Delayed = typename DelayedImpl<Class, T...>::type;

#define USING_MEMBER(member)                                                                                                                \
    template<typename... T>                                                                                                                 \
    auto member(T &&... args) -> decltype(DelegateType(std::declval<Delayed<decltype(*this), T...>>()).member(std::forward<T>(args)...))    \
    {                                                                                                                                       \
        return delegate(*this).member(std::forward<T>(args)...);                                                                            \
    }                                                                                                                                       \

template<typename Delegate, class Derived>
struct Forwarder
{
    template<typename Self, typename MemberPointer, typename... Args>
    auto operator()(Self &self, MemberPointer member_pointer, Args &&... args)
    {
        static_assert(std::is_base_of<Self, Derived>::value);

        //! Downcast to the derived class
        auto &derived = static_cast<Derived &>(self);

        //! Now invoke the conversion operator
        auto &delegate = static_cast<Delegate &>(derived);

        return (delegate.*member_pointer)(std::forward<decltype(args)>(args)...);
    }
};

template<typename Delegate, class Derived>
struct MemberFunctions {};

template<typename Delegate, class Derived, size_t index>
using using_member_t = typename reflection::reflected_member_t<Delegate, index>::template reflect<Forwarder<Delegate, Derived>>;

template<typename Delegate, class Derived, typename index_pack>
struct ReflectedMemberFunctionsImpl;

template<typename Delegate, class Derived, size_t... indices>
struct ReflectedMemberFunctionsImpl<Delegate, Derived, std::index_sequence<indices...>>
    : using_member_t<Delegate, Derived, indices>...
{
};

template<typename Delegate, class Derived>
using ReflectedMemberFunctions = ReflectedMemberFunctionsImpl<
    Delegate,
    Derived,
    std::make_index_sequence<reflection::reflected_member_count_v<Delegate>>>;

template<typename Delegate, class Derived, size_t index>
using using_class_member_t = typename reflection::reflected_class_member_t<Delegate, index>::template reflect<Forwarder<Delegate, Derived>>;

template<typename Delegate, class Derived, typename index_pack>
struct ReflectedClassMemberFunctionsImpl;

template<typename Delegate, class Derived, size_t... indices>
struct ReflectedClassMemberFunctionsImpl<Delegate, Derived, std::index_sequence<indices...>>
    : using_class_member_t<Delegate, Derived, indices>...
{
};

template<typename Delegate, class Derived>
using ReflectedClassMemberFunctions = ReflectedClassMemberFunctionsImpl<
    Delegate,
    Derived,
    std::make_index_sequence<reflection::reflected_class_member_count_v<Delegate>>>;

template<typename MemberTypeTag, typename Delegate, typename = void>
struct member_type_introducer
{
    template<typename T>
    struct type
    {
    };
};

template<typename...>
using void_t = void;

#define DECLARE_USING_MEMBER_TYPE(name)                                                     \
    struct tag_##name;                                                                      \
                                                                                            \
    template<typename Delegate>                                                             \
    struct member_type_introducer<tag_##name, Delegate, void_t<typename Delegate::name>>    \
    {                                                                                       \
        template<typename T>                                                                \
        struct type                                                                         \
        {                                                                                   \
            using name = typename T::name;                                                  \
        };                                                                                  \
    }                                                                                       \

#define USING_MEMBER_TYPE(name)                                             \
    member_type_introducer<tag_##name, Delegate>::template type<Delegate>   \

DECLARE_USING_MEMBER_TYPE(value_type);

template<typename Delegate, class Derived>
struct STL : USING_MEMBER_TYPE(value_type)
{
    USING_MEMBER(push_back)
    USING_MEMBER(begin)
    USING_MEMBER(end)
};

template<typename Delegate, class Derived>
struct using_base
{
    operator Delegate &()
    {
        auto &derived = static_cast<Derived &>(*this);
        return static_cast<Delegate &>(derived);
    }
};

template<typename Delegate>
struct using_base<Delegate, void>
{
    // TODO: Add some checks to see whether this doesn't add too much overhead
    //       for this specific Delegate type.
    virtual operator Delegate &() = 0;
};

template<typename Delegate, class Derived>
class using_ : public using_base<Delegate, Derived>,
               public MemberFunctions<Delegate, using_<Delegate, Derived>>,
               public STL<Delegate, using_<Delegate, Derived>>,
               public ReflectedMemberFunctions<Delegate, using_<Delegate, Derived>>,
               public ReflectedClassMemberFunctions<Delegate, using_<Delegate, Derived>>
{
};

} // namespace smartref
