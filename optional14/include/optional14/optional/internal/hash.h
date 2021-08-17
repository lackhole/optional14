# /*
#  * Created by YongGyu Lee on 2021/05/25.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_INTERNAL_HASH_H_
# define OPTIONAL14_OPTIONAL_INTERNAL_HASH_H_
#
# include <functional>
# include <type_traits>

namespace optional14 {
namespace internal {
namespace optional {

struct not_constructible {
  not_constructible() = delete;
};

struct constructible {};

template<typename T>
using hash_constructible =
typename std::conditional<
  std::is_default_constructible<std::hash<T>>::value,
  constructible,
  not_constructible>::type;

} // namespace optional
} // namespace internal
} // namespace optional14

# endif //OPTIONAL14_OPTIONAL_INTERNAL_HASH_H_
