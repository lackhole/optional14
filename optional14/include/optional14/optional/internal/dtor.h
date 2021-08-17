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
# include "optional14/optional/internal/macro.h"

namespace optional14 {
namespace internal {
namespace optional {

template<typename T, bool v = std::is_trivially_destructible<T>::value>
struct dtor {
  using value_type = T;

  constexpr dtor() noexcept
    : null{} {}

  template<typename ...Args>
  constexpr explicit dtor(in_place_t, Args&&... args)
    : val(std::forward<Args>(args)...),
      valid(true) {}

  void reset() {
    if (valid) {
      val.value_type::~value_type();
      valid = false;
    }
  }

  constexpr const value_type* pointer() const noexcept { return addressof(val); }
  OP_CONSTEXPR    value_type* pointer()       noexcept { return addressof(val); }

  constexpr const value_type& ref() const&  noexcept { return val;       }
  OP_CONSTEXPR    value_type& ref()      &  noexcept { return val;       }
  constexpr const value_type& ref() const&& noexcept { return std::move(val); }
  OP_CONSTEXPR    value_type& ref()      && noexcept { return std::move(val); }

  template<typename ...Args>
  void construct(Args&&... args) {
    ::new((void*)std::addressof(val)) value_type(std::forward<Args>(args)...);
    valid = true;
  }

  template<typename Other>
  void construct_if(Other&& other) {
    if (other)
      construct(*std::forward<Other>(other));
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
  constexpr explicit dtor(in_place_t, Args&&... args)
    : val(std::forward<Args>(args)...),
      valid(true) {}

  void reset() {
    if (valid) {
      val.T::~T();
      valid = false;
    }
  }

  constexpr const value_type* pointer() const noexcept { return addressof(val); }
  OP_CONSTEXPR    value_type* pointer()       noexcept { return addressof(val); }

  constexpr const value_type& ref() const&  noexcept { return val;        }
  OP_CONSTEXPR    value_type& ref()      &  noexcept { return val;        }
  constexpr const value_type& ref() const&& noexcept { return std::move(val);  }
  OP_CONSTEXPR    value_type& ref()      && noexcept { return std::move(val);  }

  template<typename ...Args>
  void construct(Args&&... args) {
    ::new((void*)std::addressof(val)) value_type(std::forward<Args>(args)...);
    valid = true;
  }

  template<typename Other>
  void construct_if(Other&& other) {
    if (other)
      construct(*std::forward<Other>(other));
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
