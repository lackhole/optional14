# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_INTERNAL_IS_SWAPPABLE_H_
# define OPTIONAL14_OPTIONAL_INTERNAL_IS_SWAPPABLE_H_
#
# include <algorithm>
# include <type_traits>
# include <utility>

namespace optional14 {
namespace internal {
namespace optional {

template<typename T>
using void_t = void;

namespace detail {
using std::swap;

template<typename T, typename U, typename = void>
struct is_swappable_with_impl : std::false_type {};

template<typename T, typename U>
struct is_swappable_with_impl<T, U, void_t<
  decltype((swap(std::declval<T>(), std::declval<U>()),
            swap(std::declval<U>(), std::declval<T>())))
>> : std::true_type {};
} // namespace detail

template<typename T, typename = void>
struct is_referencable : std::false_type {};

template<typename T>
struct is_referencable<T, void_t<T&>> : std::true_type {};

template<typename T, typename U>
using is_swappable_with = detail::is_swappable_with_impl<T, U>;

template<typename T>
struct is_swappable :
  std::conditional_t<
    !is_referencable<T>::value,
    std::false_type,
    is_swappable_with<
      std::add_lvalue_reference_t<T>,
      std::add_lvalue_reference_t<T>>
  >{};

} // namespace optional
} // namespace internal
} // namespace optional14

# endif //OPTIONAL14_OPTIONAL_INTERNAL_IS_SWAPPABLE_H_
