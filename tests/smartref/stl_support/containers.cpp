#include <reflection/reflection.h>
#include <smartref/smartref.h>

#include <vector>

namespace tests_containers {

template<typename T>
struct Ref : smartref::using_<T>
{
  T ref;

  operator T &()
  {
    return ref;
  }
};

using T = std::vector<int>;

using reflection::reflect;

//! Member-types
constexpr auto is_valid = [](auto expression)
{
  auto expected = reflect<decltype(expression(std::declval<    T >()))>;
  auto actual   = reflect<decltype(expression(std::declval<Ref<T>>()))>;

  return actual == expected;
};

#define IS_VALID(expression) is_valid([](auto &&_) {_.expression;})

static_assert(reflect<Ref<T>::value_type>             == reflect<T::value_type>);
static_assert(reflect<Ref<T>::allocator_type>         == reflect<T::allocator_type>);
static_assert(reflect<Ref<T>::size_type>              == reflect<T::size_type>);
static_assert(reflect<Ref<T>::difference_type>        == reflect<T::difference_type>);
static_assert(reflect<Ref<T>::reference>              == reflect<T::reference>);
static_assert(reflect<Ref<T>::const_reference>        == reflect<T::const_reference>);
static_assert(reflect<Ref<T>::pointer>                == reflect<T::pointer>);
static_assert(reflect<Ref<T>::const_pointer>          == reflect<T::const_pointer>);
static_assert(reflect<Ref<T>::iterator>               == reflect<T::iterator>);
static_assert(reflect<Ref<T>::const_iterator>         == reflect<T::const_iterator>);
static_assert(reflect<Ref<T>::reverse_iterator>       == reflect<T::reverse_iterator>);
static_assert(reflect<Ref<T>::const_reverse_iterator> == reflect<T::const_reverse_iterator>);

//! Member functions
// TODO: Test all overloads
// TODO: (constructor)
// TODO: operator=
static_assert(IS_VALID(assign(0, 0)));
static_assert(IS_VALID(assign(begin(_), end(_))));
static_assert(IS_VALID(get_allocator()));

//! Element access
static_assert(IS_VALID(at(T::size_type{})));
// TODO: operator[]
static_assert(IS_VALID(front()));
static_assert(IS_VALID(back()));
static_assert(IS_VALID(data()));

//! Iterators
static_assert(IS_VALID(begin()));
static_assert(IS_VALID(cbegin()));
static_assert(IS_VALID(end()));
static_assert(IS_VALID(cend()));
static_assert(IS_VALID(rbegin()));
static_assert(IS_VALID(crbegin()));
static_assert(IS_VALID(rend()));
static_assert(IS_VALID(crend()));

//! Capacity
static_assert(IS_VALID(empty()));
static_assert(IS_VALID(size()));
static_assert(IS_VALID(max_size()));
static_assert(IS_VALID(reserve(T::size_type{})));
static_assert(IS_VALID(capacity()));
static_assert(IS_VALID(shrink_to_fit()));

//! Modifiers
static_assert(IS_VALID(clear()));
static_assert(IS_VALID(insert(begin(_), 0)));
static_assert(IS_VALID(emplace(begin(_), 0)));
static_assert(IS_VALID(template emplace<int>(begin(_), 0)));
static_assert(IS_VALID(erase(begin(_))));
static_assert(IS_VALID(push_back(0)));
static_assert(IS_VALID(emplace_back(0)));
static_assert(IS_VALID(template emplace_back<int>(0)));
static_assert(IS_VALID(pop_back()));
static_assert(IS_VALID(resize(T::size_type{})));
static_assert(IS_VALID(swap(_)));

//! Non-member functions
// TODO: operator==
// TODO: operator!=
// TODO: operator<
// TODO: operator<=
// TODO: operator>
// TODO: operator>=
// TODO: std::swap

// TODO: Deduction guides

} // namespace tests_containers
