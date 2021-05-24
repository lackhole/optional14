# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
#ifndef OPTIONAL14_OPTIONAL_INTERNAL_MOVE_ASSIGNMENT_H_
#define OPTIONAL14_OPTIONAL_INTERNAL_MOVE_ASSIGNMENT_H_
#
# include <type_traits>
#
# include "optional14/optional/internal/copy_assignment.h"
# include "optional14/optional/internal/special_mf_category.h"

namespace optional14 {
namespace internal {
namespace optional {

template<typename T>
using move_assign_category_t = conditional3_t<
  std::is_trivially_move_constructible<T>::value &&
  std::is_trivially_move_assignable<T>::value &&
  std::is_trivially_destructible<T>::value,
    special_mf_trivial_t,
  std::is_move_constructible<T>::value &&
  std::is_move_assignable<T>::value,
    special_mf_user_defined_t,
  special_mf_deleted_t>;

template<typename T, typename Category = move_assign_category_t<T>>
struct move_assign : copy_assign<T> {
  using base = copy_assign<T>;
  using base::base;
};

template<typename T>
struct move_assign<T, special_mf_user_defined_t> : copy_assign<T> {
  using base = copy_assign<T>;
  using base::base;

  move_assign() = default;
  move_assign(move_assign const&) = default;
  move_assign(move_assign &&) = default;
  move_assign& operator=(move_assign const&) = default;
  move_assign& operator=(move_assign && other)
    noexcept(std::is_nothrow_move_assignable<T>::value && std::is_nothrow_move_constructible<T>::value)
  {
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
};

template<typename T>
struct move_assign<T, special_mf_deleted_t> : copy_assign<T> {
  using base = copy_assign<T>;
  using base::base;

  move_assign() = default;
  move_assign(move_assign const&) = default;
  move_assign& operator=(move_assign const&) = default;
};

} // namespace optional
} // namespace internal
} // namespace optional14

# endif // OPTIONAL14_OPTIONAL_INTERNAL_MOVE_ASSIGNMENT_H_
