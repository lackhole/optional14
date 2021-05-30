# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_OPTIONAL_H_
# define OPTIONAL14_OPTIONAL_OPTIONAL_H_
#
# include <type_traits>
# include <new>
# include <utility>
#
# include "optional14/optional/internal/check_overload.h"
# include "optional14/optional/internal/move_assignment.h"
# include "optional14/optional/internal/traits.h"
# include "optional14/optional/internal/constexpr.h"
# include "optional14/optional/bad_optional_access.h"
# include "optional14/optional/inplace_t.h"
# include "optional14/optional/nullopt_t.h"
# include "optional14/optional/swap.h"

namespace optional14 {

template<typename T>
class optional :
 private internal::optional::move_assign<T>,
 private internal::optional::check_copy_constructible<T>,
 private internal::optional::check_move_constructible<T>,
 private internal::optional::check_copy_assignable<T>,
 private internal::optional::check_move_assignable<T> {
  using base = internal::optional::move_assign<T>;
  using base::base;

 public:
  using value_type = T;

  static_assert(!std::is_reference<T>::value,
    "optional14::optional : T must not be a reference type");
  static_assert(!std::is_same<typename std::remove_reference<typename std::decay<T>::type>::type, nullopt_t>::value,
    "optional14::optional : T must not be optional14::nullopt_t");
  static_assert(!std::is_same<typename std::remove_reference<typename std::decay<T>::type>::type, in_place_t>::value,
    "optional14::optional : T must not be optional14::in_place_t");
  static_assert(std::is_destructible<T>::value,
    "optional14::optional : T must be destructible");
  static_assert(!std::is_array<T>::value,
    "optional14::optional : T must be an array type");

  // constructors

  constexpr optional() noexcept = default;
  constexpr optional(nullopt_t) noexcept {};
  constexpr optional(const optional& other) = default;
  constexpr optional(optional&& other) = default;

  template<typename U,
    typename std::enable_if<
      std::is_constructible<value_type, const U&>::value &&
      internal::optional::check_constructible<value_type, optional<U>>::value &&
      internal::optional::check_convertible  <value_type, optional<U>>::value,
    int>::type = 0>
  OPTIONAL14_CONSTEXPR optional(const optional<U>& other) {
    this->construct_if(*other);
  }

  template<typename U,
    typename std::enable_if<
      std::is_constructible<value_type, const U&>::value &&
      internal::optional::check_constructible<value_type, optional<U>>::value &&
      internal::optional::check_convertible  <value_type, optional<U>>::value &&
      !std::is_convertible<const U&, value_type>::value,
    int>::type = 0>
  explicit OPTIONAL14_CONSTEXPR optional(const optional<U>& other) {
    this->construct_if(*other);
  }

  template<typename U,
    typename std::enable_if<
      std::is_constructible<value_type, U&&>::value &&
      internal::optional::check_constructible<value_type, optional<U>>::value &&
      internal::optional::check_convertible  <value_type, optional<U>>::value,
    int>::type = 0>
  OPTIONAL14_CONSTEXPR optional(optional<U>&& other) {
    this->construct_if(std::move(*other));
  }

  template<typename U,
    typename std::enable_if<
      std::is_constructible<value_type, U&&>::value &&
      internal::optional::check_constructible<value_type, optional<U>>::value &&
      internal::optional::check_convertible  <value_type, optional<U>>::value &&
      !std::is_convertible<U&&, value_type>::value,
    int>::type = 0>
  explicit OPTIONAL14_CONSTEXPR optional(optional<U>&& other) {
    this->construct_if(std::move(*other));
  }

  // Separated into 2 overloads to prevent MSVC from making an ambiguous call in C++14
  template<typename std::enable_if<std::is_constructible<value_type>::value, int>::type = 0>
  constexpr explicit optional(in_place_t)
    : base(in_place) {}

  template<typename Arg, typename ...Args,
    typename std::enable_if<
      std::is_constructible<value_type, Arg, Args...>::value,
    int>::type = 0>
  constexpr explicit optional(in_place_t, Arg&& arg, Args&&... args)
    : base(in_place, std::forward<Arg>(arg), std::forward<Args>(args)...) {}

  template<typename U, typename ...Args,
    typename std::enable_if<
      std::is_constructible<value_type, std::initializer_list<U>&, Args&&...>::value,
    int>::type = 0>
  constexpr explicit optional(in_place_t, std::initializer_list<U> ilist, Args&&... args)
    : base(in_place, ilist, std::forward<Args>(args)...) {}

  template<typename U = value_type,
    typename std::enable_if<
      std::is_constructible<value_type,U&&>::value &&
      !std::is_same<internal::optional::strip_t<U>, in_place_t>::value &&
      !std::is_same<internal::optional::strip_t<U>, optional<value_type>>::value,
    int>::type = 0>
  constexpr optional(U&& value)
    : base(in_place, std::forward<U>(value)) {}

  // assignment operators

  optional& operator=(nullopt_t) noexcept {
    this->reset();
  }

  OPTIONAL14_CONSTEXPR optional& operator=(optional const&) = default;
  OPTIONAL14_CONSTEXPR optional& operator=(optional &&) = default;

  template<typename U,
    typename std::enable_if<
      (std::is_constructible<value_type, U>::value &&
       std::is_assignable<value_type&, U>::value &&
      !std::is_same<internal::optional::strip_t<U>, optional>::value) &&
      (!std::is_scalar<value_type>::value ||
       !std::is_same<typename std::decay<U>::type, value_type>::value),
    int>::type = 0>
  optional& operator=(U&& value) {
    if (has_value()) {
      this->val = std::forward<U>(value);
    } else {
      this->construct(std::forward<U>(value));
    }
    return *this;
  }

  template<typename U,
    typename std::enable_if<
      internal::optional::check_constructible<value_type, optional<U>>::value &&
      internal::optional::check_convertible  <value_type, optional<U>>::value &&
      internal::optional::check_assignable   <value_type, optional<U>>::value &&
      std::is_constructible<value_type , const U&>::value &&
      std::is_assignable   <value_type&, const U&>::value,
    int>::type = 0>
  optional& operator=(const optional<U>& other) {
    if (other.has_value()) {
      if (this->has_value())
        this->val = *other;
      else
        this->construct(*other);
    } else { // !other.has_value()
      if (this->has_value())
        this->reset();
    }
  }

  template<typename U,
    typename std::enable_if<
      internal::optional::check_constructible<value_type, optional<U>>::value &&
      internal::optional::check_convertible  <value_type, optional<U>>::value &&
      internal::optional::check_assignable   <value_type, optional<U>>::value &&
      std::is_constructible<value_type , U>::value &&
      std::is_assignable   <value_type&, U>::value,
    int>::type = 0>
  optional& operator=(optional<U>&& other) {
    if (other.has_value()) {
      if (this->has_value())
        this->val = std::move(*other);
      else
        this->construct(std::move(*other));
    } else { // !other.has_value()
      if (this->has_value())
        this->reset();
    }
  }


  OPTIONAL14_CONSTEXPR inline const value_type*  operator->() const { return this->pointer(); }
  OPTIONAL14_CONSTEXPR inline       value_type*  operator->()       { return this->pointer(); }

  OPTIONAL14_CONSTEXPR inline const value_type&  operator*() const&  { return this->ref(); }
  OPTIONAL14_CONSTEXPR inline       value_type&  operator*()      &  { return this->ref(); }
  OPTIONAL14_CONSTEXPR inline const value_type&& operator*() const&& { return std::move(this->ref()); }
  OPTIONAL14_CONSTEXPR inline       value_type&& operator*()      && { return std::move(this->ref()); }

  constexpr inline explicit operator bool() const noexcept {
    return this->valid;
  }
  constexpr inline bool has_value() const noexcept {
    return this->valid;
  }

  OPTIONAL14_CONSTEXPR inline value_type& value() & {
    if (!this->has_value())
      throw bad_optional_access{};
    return this->ref();
  }
  OPTIONAL14_CONSTEXPR inline const value_type& value() const& {
    if (!this->has_value())
      throw bad_optional_access{};
    return this->ref();
  }
  OPTIONAL14_CONSTEXPR inline value_type&& value() && {
    if (!this->has_value())
      throw bad_optional_access{};
    return std::move(this->ref());
  }
  OPTIONAL14_CONSTEXPR inline const value_type&& value() const && {
    if (!this->has_value())
      throw bad_optional_access{};
    return std::move(this->ref());
  }

  template<typename U>
  OPTIONAL14_CONSTEXPR value_type value_or(U&& default_value) const & {
    static_assert(std::is_copy_constructible<value_type>::value,
      "optional14::optional<T>::value_or : T must be copy constructible");
    static_assert(std::is_convertible<U&&, value_type>::value,
      "optional14::optional<T>::value_or : U&& must be convertible to T");

    return this->has_value() ? **this : static_cast<value_type>(std::forward<U>(default_value));
  }

  template<typename U>
  OPTIONAL14_CONSTEXPR value_type value_or(U&& default_value) && {
    static_assert(std::is_move_constructible<value_type>::value,
                  "optional14::optional<T>::value_or : T must be move constructible");
    static_assert(std::is_convertible<U&&, value_type>::value,
                  "optional14::optional<T>::value_or : U&& must be convertible to T");

    return this->has_value() ? std::move(**this) : static_cast<value_type>(std::forward<U>(default_value));
  }

  void swap(optional& other)
#if __cplusplus >= 201703
    noexcept(std::is_nothrow_move_constructible<value_type>::value && std::is_nothrow_swappable<value_type>::value)
#else
    noexcept(std::is_nothrow_move_constructible<value_type>::value)
#endif
  {
    static_assert(std::is_move_constructible<value_type>::value,
      "optional14::optional<T>::swap : T must be move constructible");

    if (other.has_value()) {
      if (this->has_value()) {
        std::swap(**this, *other);
      } else { // !this->has_value()
        this->construct(std::move(*other));
        other.reset();
      }
    } else {
      if (this->has_value()) {
        other.construct(**this);
        this->reset();
      }
    }
  }

  using base::reset;

  template<typename ...Args, typename std::enable_if<std::is_constructible<value_type, Args...>::value, int>::type = 0>
  value_type& emplace(Args&&... args) {
    this->reset();
    this->construct(std::forward<Args>(args)...);
  }

  template<typename U, typename ...Args,
    typename std::enable_if<
      std::is_constructible<value_type, std::initializer_list<U>&, Args&&...>::value,
    int>::type = 0>
  value_type& emplace(std::initializer_list<U> ilist, Args&&... args) {
    this->reset();
    this->construct(ilist, std::forward<Args>(args)...);
  }

};

# if __cplusplus >= 201703
template<class T> optional(T) -> optional<T>;
# endif

template<typename T>
OPTIONAL14_CONSTEXPR inline optional<typename std::decay<T>::type> make_optional(T&& value) {
  return optional<typename std::decay<T>::type>(std::forward<T>(value));
}

template<typename T, typename ...Args>
OPTIONAL14_CONSTEXPR inline optional<T> make_optional(Args&&... args) {
  return optional<T>(in_place, std::forward<Args>(args)...);
}

template<typename T, typename U, typename ...Args>
OPTIONAL14_CONSTEXPR inline optional<T> make_optional(std::initializer_list<U> ilist, Args&&... args) {
  return optional<T>(in_place, ilist, std::forward<Args>(args)...);
}

// compare two optional objects

template<typename T, typename U >
OPTIONAL14_CONSTEXPR bool operator==(const optional<T>& lhs, const optional<U>& rhs) {
  if (bool(lhs) != bool(rhs))
    return false;
  if (!lhs)
    return true;
  return *lhs == *rhs;
}

template<typename T, typename U >
OPTIONAL14_CONSTEXPR bool operator!=(const optional<T>& lhs, const optional<U>& rhs) {
  if (bool(lhs) != bool(rhs))
    return true;
  if (!lhs)
    return false;
  return lhs != rhs;
}

template<typename T, typename U >
OPTIONAL14_CONSTEXPR bool operator<(const optional<T>& lhs, const optional<U>& rhs) {
  if (!rhs.has_value())
    return false;
  if (!lhs.has_value())
    return true;
  return *lhs < *rhs;
}

template<typename T, typename U >
OPTIONAL14_CONSTEXPR bool operator<=(const optional<T>& lhs, const optional<U>& rhs) {
  if (!lhs.has_value())
    return true;
  if (!rhs.has_value())
    return false;
  return *lhs <= *rhs;
}

template<typename T, typename U >
OPTIONAL14_CONSTEXPR bool operator>(const optional<T>& lhs, const optional<U>& rhs) {
  if (!lhs.has_value())
    return false;
  if (!rhs.has_value())
    return true;
  return *lhs > *rhs;
}

template<typename T, typename U >
OPTIONAL14_CONSTEXPR bool operator>=(const optional<T>& lhs, const optional<U>& rhs) {
  if (!rhs.has_value())
    return true;
  if (!lhs.has_value())
    return false;
  return *lhs >= *rhs;
}


// compare an optional object with a nullopt

template<typename T>
constexpr inline bool operator==(const optional<T>& opt, nullopt_t) noexcept {
  return !opt;
}

template<typename T>
constexpr inline bool operator==(nullopt_t, const optional<T>& opt) noexcept {
  return !opt;
}

template<typename T>
constexpr inline bool operator!=(const optional<T>& opt, nullopt_t) noexcept {
  return bool(opt);
}

template<typename T>
constexpr inline bool operator!=(nullopt_t, const optional<T>& opt) noexcept {
  return bool(opt);
}

template<typename T>
constexpr inline bool operator<(const optional<T>& opt, nullopt_t) noexcept {
  return false;
}

template<typename T>
constexpr inline bool operator<(nullopt_t, const optional<T>& opt) noexcept {
  return bool(opt);
}

template<typename T>
constexpr inline bool operator<=(const optional<T>& opt, nullopt_t) noexcept {
  return !opt;
}

template<typename T>
constexpr inline bool operator<=(nullopt_t, const optional<T>& opt) noexcept {
  return true;
}

template<typename T>
constexpr inline bool operator>(const optional<T>& opt, nullopt_t) noexcept {
  return bool(opt);
}

template<typename T>
constexpr inline bool operator>(nullopt_t, const optional<T>& opt) noexcept {
  return false;
}

template<typename T>
constexpr inline bool operator>=(const optional<T>& opt, nullopt_t) noexcept {
  return true;
}

template<typename T>
constexpr inline bool operator>=(nullopt_t, const optional<T>& opt) noexcept {
  return !opt;
}


// compare an optional object with a value

template<typename T, typename U>
constexpr inline bool operator==(const optional<T>& opt, const U& value) {
  return bool(opt) ? *opt == value : false;
}

template<typename T, typename U>
constexpr inline bool operator==(const T& value, const optional<U>& opt) {
  return bool(opt) ? value == *opt : false;
}

template<typename T, typename U>
constexpr inline bool operator!=(const optional<T>& opt, const U& value) {
  return bool(opt) ? opt != value : true;
}

template<typename T, typename U>
constexpr inline bool operator!=(const T& value, const optional<U>& opt) {
  return bool(opt) ? value != opt : true;
}

template<typename T, typename U>
constexpr inline bool operator<(const optional<T>& opt, const U& value) {
  return bool(opt) ? opt < value : true;
}

template<typename T, typename U>
constexpr inline bool operator<(const T& value, const optional<U>& opt) {
  return bool(opt) ? value < opt : false;
}

template<typename T, typename U>
constexpr inline bool operator<=(const optional<T>& opt, const U& value) {
  return bool(opt) ? opt <= value : true;
}

template<typename T, typename U>
constexpr inline bool operator<=(const T& value, const optional<U>& opt) {
  return bool(opt) ? value <= opt : false;
}

template<typename T, typename U>
constexpr inline bool operator>(const optional<T>& opt, const U& value) {
  return bool(opt) ? opt > value : false;
}

template<typename T, typename U>
constexpr inline bool operator>(const T& value, const optional<U>& opt) {
  return bool(opt) ? value > opt : true;
}

template<typename T, typename U>
constexpr inline bool operator>=(const optional<T>& opt, const U& value) {
  return bool(opt) ? opt >= value : false;
}

template<typename T, typename U>
constexpr inline bool operator>=(const T& value, const optional<U>& opt) {
  return bool(opt) ? value >= opt : true;
}

} // namespace optional14

# endif // OPTIONAL14_OPTIONAL_OPTIONAL_H_
