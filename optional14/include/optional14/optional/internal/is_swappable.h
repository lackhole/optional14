# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_INTERNAL_IS_SWAPPABLE_H_
# define OPTIONAL14_OPTIONAL_INTERNAL_IS_SWAPPABLE_H_
#
# include <type_traits>
# include <utility>

namespace optional14 {
namespace internal {
namespace optional {

template<typename T>
using void_t = void;

template<typename T, typename = void>
struct is_swappable : std::false_type {};

template<typename T>
struct is_swappable<T, void_t<
  decltype(std::swap(std::declval<T>(), std::declval<T>()))
>> : std::true_type {};

} // namespace optional
} // namespace internal
} // namespace optional14

# endif //OPTIONAL14_OPTIONAL_INTERNAL_IS_SWAPPABLE_H_
