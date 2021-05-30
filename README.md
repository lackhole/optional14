# Optional14

## C++14 implementation of std::optional
* Standalone (boost **not** required. Used C++14 STL only)
* Header only
* You can get a integrated single header file at [here](https://github.com/lackhole/optional14/releases)

[![Ubuntu](https://github.com/lackhole/optional14/actions/workflows/ubuntu.yml/badge.svg?branch=main)](https://github.com/lackhole/optional14/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/lackhole/optional14/actions/workflows/windows.yml/badge.svg?branch=main)](https://github.com/lackhole/optional14/actions/workflows/windows.yml)

```cpp
namespace o14 = optional14;

o14::optional<std::string> op;

std::cout << op.value_or("null null") << '\n';  // null null
op = "Hello, world!";
std::cout << op.value_or("null null") << '\n';  // Hello, world!
op = {};
std::cout << op.value_or("null null") << '\n';  // null null
```

```cpp
std::map<o14::optional<int>, std::string> m = {
    {3, "three"}, {5, "five"}, {o14::nullopt, "null"}, {1, "one"}
};

for (const auto &p : m) {
  std::cout << p.first.value_or(-100) << " : " << p.second << '\n';
}
// output:
//  -100 : null
//  1 : one
//  3 : three
//  5 : five
```

```cpp
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
```

```cpp
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
```
[example code](main.cc)  

### Notes
* This project meets the all requirements of the [current STL](https://en.cppreference.com/w/cpp/utility/optional) except for the [`std::is_nothrow_swappable<value_type>::value`](https://github.com/lackhole/optional14/blob/main/optional14/include/optional14/optional/optional.h#L224),
    since there is no way to implement this in C++14
* This project specializes `std::hash` and overloads `std::swap` for type `optional14::optional<T>` without conflicts.  
  If you're not willing for this to do so you can change their namespaces.
    * [hash.h](https://github.com/lackhole/optional14/blob/main/optional14/include/optional14/optional/hash.h#L16)
    * [swap.h](https://github.com/lackhole/optional14/blob/main/optional14/include/optional14/optional/swap.h#L15)

### C++11 implementation: WIP
