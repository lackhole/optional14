# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_NULLOPT_T_H_
# define OPTIONAL14_OPTIONAL_NULLOPT_T_H_

namespace optional14 {

struct nullopt_t {
  constexpr nullopt_t(int) {}
};

constexpr nullopt_t nullopt{0};

}

# endif // OPTIONAL14_OPTIONAL_NULLOPT_T_H_
