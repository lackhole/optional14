# /*
#  * Created by YongGyu Lee on 2021/05/23.
#  */
#
# ifndef OPTIONAL14_OPTIONAL_BAD_OPTIONAL_ACCESS_H_
# define OPTIONAL14_OPTIONAL_BAD_OPTIONAL_ACCESS_H_
#
# include <exception>

namespace optional14 {

class bad_optional_access : public std::exception {
 public:
  bad_optional_access() noexcept = default;
  bad_optional_access(const bad_optional_access&) noexcept = default;
  bad_optional_access& operator=(const bad_optional_access&) noexcept = default;

  const char *what() const noexcept override {
    return "bad optional access";
  }
};

}

# endif // OPTIONAL14_OPTIONAL_BAD_OPTIONAL_ACCESS_H_
