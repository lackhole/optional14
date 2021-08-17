# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_SWAP_H_
# define OPTIONAL14_OPTIONAL_SWAP_H_
#
# include <algorithm>
# include <type_traits>
# include <utility>
#
# include "optional14/optional/internal/is_swappable.h"
# include "optional14/optional/forward_declare.h"

namespace std {

template<typename T,
  typename ::std::enable_if<
    ::std::is_move_constructible<T>::value && ::optional14::internal::optional::is_swappable<T>::value,
  int>::type = 0>
void swap(::optional14::optional<T>& lhs, ::optional14::optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

} // namespace std

# endif // OPTIONAL14_OPTIONAL_SWAP_H_
