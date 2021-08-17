# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_HASH_H_
# define OPTIONAL14_OPTIONAL_HASH_H_
#
# include <cstddef>
#
# include <functional>
# include <type_traits>
#
# include "optional14/optional/internal/hash.h"
# include "optional14/optional/optional.h"

namespace std {

template<typename T>
struct hash<optional14::optional<T>>
  : optional14::internal::optional::hash_constructible<typename std::remove_const<T>::type> {
  using argument_type = optional14::optional<T>;
  using result_type = std::size_t;

  result_type operator()(const argument_type& key) const {
    return key.has_value() ? ::std::hash<typename std::remove_const<T>::type>()(*key) : 0;
  }
};

} // namespace std

# endif //OPTIONAL14_OPTIONAL_HASH_H_
