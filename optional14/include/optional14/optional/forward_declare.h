# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_FORWARD_DECLARE_H_
# define OPTIONAL14_OPTIONAL_FORWARD_DECLARE_H_

namespace optional14 {

template<typename T>
class optional;

template<typename T>
inline void swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs)));


} // namespace optional14

# endif // OPTIONAL14_OPTIONAL_FORWARD_DECLARE_H_
