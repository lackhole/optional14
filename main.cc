//
// Created by YongGyu Lee on 2021/05/25.
//

#include <iostream>
#include <string>
#include <functional>
#include <map>

#include "optional14/optional.h"


namespace o14 = optional14;

int main() {

  {
    o14::optional<std::string> op;

    std::cout << op.value_or("null null") << '\n';
    op = "Hello, world!";
    std::cout << op.value_or("null null") << '\n';
    op = {};
    std::cout << op.value_or("null null") << '\n';
  }

  {
    std::map<o14::optional<int>, std::string> m = {
      {3, "three"}, {5, "five"}, {o14::nullopt, "null"}, {1, "one"}
    };

    for (const auto &p : m) {
      std::cout << p.first.value_or(-100) << " : " << p.second << '\n';
    }
  }

  {
    auto div = [](int a, int b) -> o14::optional<int> {
      if (b == 0) return o14::nullopt;
      return a / b;
    };

    std::cout << "Calculating 10 / 0: ";
    try {
      std::cout << div(10, 0).value() << '\n';
    } catch (const o14::bad_optional_access& e) {
      std::cout << "Error: dividing with zero\n";
    }
  }

  {
    struct NoMove {
      NoMove() = default;
      NoMove(const NoMove&) {
        std::cout << "COPY SELECTED\n";
      }
      NoMove(NoMove&&) = delete;
      NoMove& operator=(const NoMove&) = default;
    };

    o14::optional<NoMove> op1{o14::in_place};
    o14::optional<NoMove> op2;
    std::swap(op1, op2); // copy is selected for non-movable object
  }

  return 0;
}
