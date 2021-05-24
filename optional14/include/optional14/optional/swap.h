# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_SWAP_H_
# define OPTIONAL14_OPTIONAL_SWAP_H_
#
# include <type_traits>
#
# include "optional14/optional/forward_declare.h"
# include "optional14/optional/internal/is_swappable.h"

namespace optional14 {

template<typename T,
  std::enable_if_t<
    std::is_move_constructible<T>::value && internal::optional::is_swappable<T>::value,
    int> = 0>
inline void swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

} // namespace optional14

# endif // OPTIONAL14_OPTIONAL_SWAP_H_
