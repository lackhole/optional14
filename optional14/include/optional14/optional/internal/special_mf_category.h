# /*
#  * Created by YongGyu Lee on 2021/05/24.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_INTERNAL_SPECIAL_MF_CATEGORY_H_
# define OPTIONAL14_OPTIONAL_INTERNAL_SPECIAL_MF_CATEGORY_H_
#
# include <type_traits>

namespace optional14 {
namespace internal {
namespace optional {

template<bool Cond1, typename If, bool Cond2, typename ElseIf, typename Else>
struct conditional3 {
  using type = std::conditional_t<
    !Cond1,
    std::conditional_t<!Cond2, Else, ElseIf>,
    If
  >;
};

template<bool Cond1, typename If, bool Cond2, typename ElseIf, typename Else>
using conditional3_t = typename conditional3<Cond1, If, Cond2, ElseIf, Else>::type;

struct special_mf_trivial_t {};
struct special_mf_user_defined_t {};
struct special_mf_deleted_t {};

} // namespace optional
} // namespace internal
} // namespace optional14

# endif // OPTIONAL14_OPTIONAL_INTERNAL_SPECIAL_MF_CATEGORY_H_
