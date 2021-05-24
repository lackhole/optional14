# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_INTERNAL_DTOR_H_
# define OPTIONAL14_OPTIONAL_INTERNAL_DTOR_H_
#
# include <type_traits>
# include <utility>
#
# include "optional14/optional/inplace_t.h"

namespace optional14 {
namespace internal {
namespace optional {

template<typename T, bool v = std::is_trivially_destructible<T>::value>
struct dtor {
  using value_type = T;

  constexpr dtor() noexcept
    : null{} {}

  template<typename ...Args>
  constexpr dtor(in_place_t, Args&&... args)
    : val(std::forward<Args>(args)...),
      valid(true) {}

  void reset() {
    if (valid) {
      val.value_type::~value_type();
      valid = false;
    }
  }

  inline const value_type* pointer() const { return &val; }
  inline       value_type* pointer()       { return &val; }

  inline const value_type& ref() const&  { return val;             }
  inline       value_type& ref()      &  { return val;             }
  inline const value_type& ref() const&& { return std::move(val);  }
  inline       value_type& ref()      && { return std::move(val);  }

  template<typename ...U>
  void construct(U&&... args) {
    ::new((void*)&val) value_type(std::forward<U>(args)...);
    valid = true;
  }

  template<typename U>
  void construct_if(U&& arg) {
    if (valid)
      construct(std::forward<U>(arg));
  }

  ~dtor() = default;

  union {
    char null;
    value_type val;
  };
  bool valid = false;
};

template<typename T>
struct dtor<T, false> {
  using value_type = T;

  constexpr dtor() noexcept
    : null{} {}

  template<typename ...Args>
  constexpr dtor(in_place_t, Args&&... args)
    : val(std::forward<Args>(args)...),
      valid(true) {}

  inline void reset() {
    if (valid) {
      val.T::~T();
      valid = false;
    }
  }

  inline const value_type* pointer() const { return &val; }
  inline       value_type* pointer()       { return &val; }

  inline const value_type& ref() const&  { return val;             }
  inline       value_type& ref()      &  { return val;             }
  inline const value_type& ref() const&& { return std::move(val);  }
  inline       value_type& ref()      && { return std::move(val);  }

  template<typename U>
  void construct(U&& arg) {
    ::new((void*)&val) value_type(std::forward<U>(arg));
    valid = true;
  }

  ~dtor() {
    if (valid)
      val.T::~T();
  };

  union {
    char null;
    T val;
  };
  bool valid = false;
};

} // namespace optional
} // namespace internal
} // namespace optional14

# endif // OPTIONAL14_OPTIONAL_INTERNAL_DTOR_H_
