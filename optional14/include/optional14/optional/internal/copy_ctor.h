# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_INTERNAL_COPY_CTOR_H_
# define OPTIONAL14_OPTIONAL_INTERNAL_COPY_CTOR_H_
#
# include <type_traits>
#
# include "optional14/optional/internal/special_mf_category.h"
# include "optional14/optional/internal/dtor.h"

namespace optional14 {
namespace internal {
namespace optional {

template<typename T>
using copy_ctor_category_t = conditional3_t<
  std::is_trivially_copy_constructible<T>::value,
    special_mf_trivial_t,
  std::is_copy_constructible<T>::value,
    special_mf_user_defined_t,
  special_mf_deleted_t>;

template<typename T, typename Category = copy_ctor_category_t<T>>
struct copy_ctor : dtor<T> {
  using base = dtor<T>;
  using base::base;
};

template<typename T>
struct copy_ctor<T, special_mf_user_defined_t> : dtor<T> {
  using base = dtor<T>;
  using base::base;

  copy_ctor() = default;
  copy_ctor(copy_ctor const& other) {
    if (other.valid) {
      this->construct(other.val);
    }
  }
  copy_ctor(copy_ctor &&) = default;
  copy_ctor& operator=(copy_ctor const&) = default;
  copy_ctor& operator=(copy_ctor &&) = default;
};

template<typename T>
struct copy_ctor<T, special_mf_deleted_t> : dtor<T> {
  using base = dtor<T>;
  using base::base;

  copy_ctor() = default;
  copy_ctor(copy_ctor const&) = delete;
  copy_ctor(copy_ctor &&) noexcept = default;
  copy_ctor& operator=(copy_ctor const&) = default;
  copy_ctor& operator=(copy_ctor &&) noexcept = default;
};

} // namespace optional
} // namespace internal
} // namespace optional14


# endif // OPTIONAL14_OPTIONAL_INTERNAL_COPY_CTOR_H_
