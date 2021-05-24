# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_INPLACE_T_H_
# define OPTIONAL14_OPTIONAL_INPLACE_T_H_

namespace optional14 {

struct in_place_t {
  constexpr explicit in_place_t() = default;
};

constexpr in_place_t in_place{};

}

# endif // OPTIONAL14_OPTIONAL_INPLACE_T_H_
