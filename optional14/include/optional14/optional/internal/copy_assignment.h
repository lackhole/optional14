# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_INTERNAL_COPY_ASSIGNMENT_H_
# define OPTIONAL14_OPTIONAL_INTERNAL_COPY_ASSIGNMENT_H_
#
# include <type_traits>
#
# include "optional14/optional/internal/special_mf_category.h"
# include "optional14/optional/internal/move_ctor.h"

namespace optional14 {
namespace internal {
namespace optional {

template<typename T>
using copy_assignment_category_t = conditional3_t<
  std::is_trivially_copy_constructible<T>::value &&
  std::is_trivially_copy_assignable<T>::value &&
  std::is_trivially_destructible<T>::value,
    special_mf_trivial_t,
  std::is_copy_constructible<T>::value && std::is_copy_assignable<T>::value,
    special_mf_user_defined_t,
  special_mf_deleted_t
>;

template<typename T, typename Category = copy_assignment_category_t<T>>
struct copy_assign : move_ctor<T> {
  using base = move_ctor<T>;
  using base::base;
};

template<typename T>
struct copy_assign<T, special_mf_user_defined_t> : move_ctor<T> {
  using base = move_ctor<T>;
  using base::base;

  copy_assign() = default;
  copy_assign(copy_assign const&) = default;
  copy_assign(copy_assign &&) = default;
  copy_assign& operator=(copy_assign const& other) {
    if (!other.valid) {
      this->reset();
    } else {
      if (this->valid)
        this->val = other.val;
      else
        this->construct(other.val);
    }
    return *this;
  }
  copy_assign& operator=(copy_assign &&) = default;
};

template<typename T>
struct copy_assign<T, special_mf_deleted_t> : move_ctor<T> {
  using base = move_ctor<T>;
  using base::base;

  copy_assign() = default;
  copy_assign(copy_assign const&) = default;
  copy_assign(copy_assign &&) = default;
  copy_assign& operator=(copy_assign const&) = delete;
  copy_assign& operator=(copy_assign &&) = default;
};

} // namespace optional
} // namespace internal
} // namespace optional14

# endif // OPTIONAL14_OPTIONAL_INTERNAL_COPY_ASSIGNMENT_H_
