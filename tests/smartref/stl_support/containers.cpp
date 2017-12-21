#include <reflection/reflection.h>
#include <smartref/smartref.h>

// #include <vector>
#include <cassert>

namespace tests_containers {

// template <typename F>
// class reflect_member_function : public reflection::reflect_base<reflection::reflected_kind::member_function> {
// private:
//     template <typename... ExplicitArgs, typename... Args>
//     decltype(auto) indirect(Args&&... args)
//     {
//         auto f = [](auto& obj, auto&&... args) { if constexpr (sizeof...(ExplicitArgs) == 0) return obj.operator=(std::forward<Args>(args)...); else if constexpr (utils::always_true<Args...>) return obj.template operator=<ExplicitArgs...>( std::forward<Args>(args)...); };
//         return F{}(*this, f, std::forward<Args>(args)...);
//     }
// public:
//     template <typename... ExplicitArgs, typename... Args>
//     auto operator=(Args&&... args) -> decltype(indirect<ExplicitArgs...>(std::forward<Args>(args)...)) { return indirect<ExplicitArgs...>(std::forward<Args>(args)...); }
// };
// template<typename T>

struct Ref : smartref::reflect_member_function<Ref>
// struct Ref : smartref::using_<T>
{
  int ref;

  static auto &counter()
  {
    static auto count = 0;
    return count;
  }

  operator int &()
  {
    ++counter();
    return ref;
  }

  Ref(int arg) : ref{arg} {}

  using Base = smartref::reflect_member_function<Ref>;
  using Base::operator=;

  Ref() = default;

  Ref(const Ref &) = default;
  Ref &operator=(const Ref &) = default;
  Ref(Ref &&) = default;
  Ref &operator=(Ref &&) = default;
};

template<typename T, typename Delegate = int>
auto test = []{
  //! Uninitialized construction
  T a;

  //! Default construction
  T b{};
  T c = {};

  //! Copy construction
  T d{a};
  T e = a;
  T f = {a};
  auto g{a};
  auto h = a;
  auto i = {a};

  //! Move construction
  T j{std::move(a)};
  T k = std::move(a);
  T l = {std::move(a)};
  auto m{T{}};
  auto n = T{};
  auto o = {T{}};

  //! Delegate type copy construction
  auto delegate = Delegate{};
  T p{delegate};
  T q = delegate;
  T r = {delegate};
  auto s{T{delegate}};
  auto t = T{delegate};
  auto u = {T{delegate}};

  //! Delegate type move construction
  T v{std::move(delegate)};
  T w = std::move(delegate);
  T x = {std::move(delegate)};
  auto y{T{std::move(delegate)}};
  auto z = T{std::move(delegate)};
  auto A = {T{std::move(delegate)}};

  //! Copy assignments
  a = b;
  a = b = c;
  a = (b = c);
  (a = b) = c;

  //! Move assignments
  a = std::move(b);
  a = b = std::move(c);
  a = (b = std::move(c));
  (a = b) = std::move(c);

  //! Delegate type copy assignments
  a = delegate;
  a = b = delegate;
  a = (b = delegate);
  (a = b) = delegate;

  //! Delegate type move assignments
  a = std::move(delegate);
  a = b = std::move(delegate);
  a = (b = std::move(delegate));
  (a = b) = std::move(delegate);

  return 0;
};

auto test_int = test<int>();

auto test_ref = []{
  Ref<int>::counter() = 0;
  test<Ref<int>>();
  assert(Ref<int>::counter() == 18);

  Ref<float>::counter() = 0;
  test<Ref<float>>();
  assert(Ref<float>::counter() == 18);

  Ref<bool>::counter() = 0;
  test<Ref<bool>>();
  assert(Ref<bool>::counter() == 18);

  return 0;
}();

// using T = int;
// using T = std::vector<int>;

// using reflection::reflect;

// //! Member-types
// constexpr auto is_valid = [](auto expression)
// {
//   // auto expected = reflect<decltype(expression(std::declval<    T >(), std::declval<    T >()))>;
//   auto actual   = reflect<decltype(expression(std::declval<Ref<T>>(), std::declval<    T >()))>;

//   return true;
//   // return actual == expected;
// };

// #define IS_VALID(expression) is_valid([](auto &&_, auto &&__) {return expression;})

// TODO:
// - constructing a smartref (e.g. Ref<int> x;)

// static_assert(reflect<Ref<T>::value_type>             == reflect<T::value_type>);
// static_assert(reflect<Ref<T>::allocator_type>         == reflect<T::allocator_type>);
// static_assert(reflect<Ref<T>::size_type>              == reflect<T::size_type>);
// static_assert(reflect<Ref<T>::difference_type>        == reflect<T::difference_type>);
// static_assert(reflect<Ref<T>::reference>              == reflect<T::reference>);
// static_assert(reflect<Ref<T>::const_reference>        == reflect<T::const_reference>);
// static_assert(reflect<Ref<T>::pointer>                == reflect<T::pointer>);
// static_assert(reflect<Ref<T>::const_pointer>          == reflect<T::const_pointer>);
// static_assert(reflect<Ref<T>::iterator>               == reflect<T::iterator>);
// static_assert(reflect<Ref<T>::const_iterator>         == reflect<T::const_iterator>);
// static_assert(reflect<Ref<T>::reverse_iterator>       == reflect<T::reverse_iterator>);
// static_assert(reflect<Ref<T>::const_reverse_iterator> == reflect<T::const_reverse_iterator>);

// //! Member functions
// // TODO: Test all overloads
// // TODO: (constructor)
// // TODO: Why does operator= work? What does it do?
// // static_assert(IS_VALID(_.operator=(_)));
// // static_assert(IS_VALID(_.operator=(__)));
// static_assert(IS_VALID(_ = __));
// static_assert(IS_VALID(_.assign(0, 0)));
// static_assert(IS_VALID(_.assign(begin(_), end(_))));
// static_assert(IS_VALID(_.get_allocator()));
// //! Element access
// static_assert(IS_VALID(_.at(T::size_type{})));
// static_assert(IS_VALID(_.operator[](0)));
// static_assert(IS_VALID(_[0]));
// static_assert(IS_VALID(_.front()));
// static_assert(IS_VALID(_.back()));
// static_assert(IS_VALID(_.data()));

// //! Iterators
// static_assert(IS_VALID(_.begin()));
// static_assert(IS_VALID(_.cbegin()));
// static_assert(IS_VALID(_.end()));
// static_assert(IS_VALID(_.cend()));
// static_assert(IS_VALID(_.rbegin()));
// static_assert(IS_VALID(_.crbegin()));
// static_assert(IS_VALID(_.rend()));
// static_assert(IS_VALID(_.crend()));

// //! Capacity
// static_assert(IS_VALID(_.empty()));
// static_assert(IS_VALID(_.size()));
// static_assert(IS_VALID(_.max_size()));
// static_assert(IS_VALID(_.reserve(T::size_type{})));
// static_assert(IS_VALID(_.capacity()));
// static_assert(IS_VALID(_.shrink_to_fit()));

// //! Modifiers
// static_assert(IS_VALID(_.clear()));
// static_assert(IS_VALID(_.insert(begin(_), 0)));
// static_assert(IS_VALID(_.emplace(begin(_), 0)));
// static_assert(IS_VALID(_.template emplace<int>(begin(_), 0)));
// static_assert(IS_VALID(_.erase(begin(_))));
// static_assert(IS_VALID(_.push_back(0)));
// static_assert(IS_VALID(_.emplace_back(0)));
// static_assert(IS_VALID(_.template emplace_back<int>(0)));
// static_assert(IS_VALID(_.pop_back()));
// static_assert(IS_VALID(_.resize(T::size_type{})));
// static_assert(IS_VALID(_.swap(_)));

// //! Non-member functions
// // static_assert(IS_VALID(_ == _);
// // TODO: operator!=
// // TODO: operator<
// // TODO: operator<=
// // TODO: operator>
// // TODO: operator>=
// // TODO: std::swap

// // TODO: Deduction guides

} // namespace tests_containers
