# Optional14

## C++14 implementation of std::optional
* Standalone (boost **not** required)
* Header only

```
namespace o14 = optional14;

o14::optional<std::string> op;

std::cout << op.value_or("null null") << '\n';  // null null
op = "Hello, world!";
std::cout << op.value_or("null null") << '\n';  // Hello, world!
op = {};
std::cout << op.value_or("null null") << '\n';  // null null
```

```
std::map<o14::optional<int>, std::string> m = {
    {3, "three"}, {5, "five"}, {o14::nullopt, "null"}, {1, "one"}
};

for (const auto &p : m) {
  std::cout << p.first.value_or(-100) << " : " << p.second << '\n';
}
// output:
  -100 : null
  1 : one
  3 : three
  5 : five
```

```
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

```
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

Met the all requirements of the [current STL](https://en.cppreference.com/w/cpp/utility/optional) except for the [`std::is_nothrow_swappable<value_type>::value`](https://github.com/lackhole/optional14/blob/main/optional14/include/optional14/optional/optional.h#L224),
    since there is no way to implement this in C++14

### C++11 implementation: WIP
