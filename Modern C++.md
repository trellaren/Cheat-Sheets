# 📚 Modern C++ Cheat Sheet

_(C++17 / C++20 / C++23 – the most widely adopted standards as of 2026)_

> **Tip:** Keep this sheet on your desk or a note‑app. Pull it up when you’re coding, refactoring, or reviewing someone else’s code.

---

## 1️⃣ Basics & Syntax

| Concept                    | Modern style (C++17+)                               | Old style                              |
| -------------------------- | --------------------------------------------------- | -------------------------------------- |
| **Variable declaration**   | `auto x = 42;` `<br>` `int y{7};`                   | `int y = 7;`                           |
| **Range‑based for**        | `for (auto &v : vec) { … }`                         | `for (size_t i=0;i<vec.size();++i){…}` |
| **Structured bindings**    | `auto [a, b] = std::pair{1,2};`                     | N/A                                    |
| **Lambda capture default** | `[&](int x){ return x+1; }`                         | N/A                                    |
| **Inline namespace**       | `namespace v1 { … } inline namespace current = v1;` | N/A                                    |
| **constexpr function**     | `constexpr int square(int n){return n*n;}`          | N/A                                    |

---

## 2️⃣ Memory & Ownership

| Feature                        | What it solves                             | Typical usage                                                                   |
| ------------------------------ | ------------------------------------------ | ------------------------------------------------------------------------------- |
| **Smart pointers**             | Avoid raw pointers, automatic deallocation | `std::unique_ptr<T> p = std::make_unique<T>();` `<br>` `std::shared_ptr<T> sp;` |
| **Move semantics**             | Transfer ownership cheaply                 | `auto b = std::move(a);`                                                        |
| **POD vs. non‑POD**            | Value semantics, trivial copy/move         | Prefer structs with no custom destructor                                        |
| **Allocator-aware containers** | Custom memory pools                        | `std::vector<T, MyAlloc> vec;`                                                  |

---

## 3️⃣ Modern Containers & Algorithms

| Container / Algorithm                      | Notes                                           |
| ------------------------------------------ | ----------------------------------------------- |
| `std::array<T,N>`                          | Fixed‑size array on stack.                      |
| `std::span<T>` (C++20)                     | Non-owning view of contiguous data.             |
| `std::vector`                              | Most common dynamic container.                  |
| `std::unordered_map`, `std::unordered_set` | Hash containers.                                |
| **Ranges** (`<ranges>`)                    | Pipe‑style transformations:`<algorithm>.views`. |
| `std::ranges::sort(vec)`                   | Equivalent to `std::sort(begin, end)`.          |
| `std::ranges::find_if`                     | Lambda filter.                                  |

---

## 4️⃣ Concurrency & Parallelism

| Feature                                 | Usage                                                                        |
| --------------------------------------- | ---------------------------------------------------------------------------- |
| **Thread** (`<thread>`)                 | `std::thread t(fn); t.join();`                                               |
| **Mutex / Lock** (`<mutex>`)            | `std::lock_guard<std::mutex> lg(mtx);`                                       |
| **Future / Promise** (`<future>`)       | Async results:`auto fut = std::async(fn);`                                   |
| **Parallel algorithms** (`<execution>`) | `std::sort(vec.begin(), vec.end(), std::less{}, std::execution::par_unseq);` |
| **Atomic types** (`<atomic>`)           | `std::atomic<int> counter{0};`                                               |

---

## 5️⃣ Modern I/O & Serialization

| Technique                       | Example                                                       |
| ------------------------------- | ------------------------------------------------------------- |
| **File streams** (`<fstream>`)  | `std::ifstream in("data.txt"); std::ofstream out("out.txt");` |
| **String stream** (`<sstream>`) | `std::ostringstream oss; oss << 42;`                          |
| **JSON (nlohmann/json)**        | `json j = {{"key", "value"}};`                                |
| **YAML, TOML, XML**             | Use libraries like yaml-cpp, toml11, pugixml.                 |

---

## 6️⃣ Templates & Metaprogramming

| Feature                     | What it does              | Example                                       |
| --------------------------- | ------------------------- | --------------------------------------------- |
| `template<typename T>`      | Generic type              | `T add(T a,T b){return a+b;}`                 |
| **Concepts** (`<concepts>`) | Compile‑time constraints  | `requires std::integral<T>`                   |
| **Requires clause**         | Inline constraint         | `auto f(T t) requires std::integral<T> { … }` |
| **constexpr if**            | Branching at compile time | `if constexpr (std::is_same_v<T, int>) { … }` |
| **Variable templates**      | Generic constants         | `template<class T> constexpr T pi = 3.1415;`  |

---

## 7️⃣ Standard Library Highlights

| Header         | Key components              | Quick reference                                |
| -------------- | --------------------------- | ---------------------------------------------- |
| `<optional>`   | `std::optional<T>`          | Represents value or none.                      |
| `<variant>`    | `std::variant<T...>`        | Type-safe union.                               |
| `<any>`        | `std::any`                  | Holds any type.                                |
| `<chrono>`     | Time points, durations      | `auto now = std::chrono::steady_clock::now();` |
| `<filesystem>` | Path manipulation, file ops | `fs::path p{"./file.txt"}; fs::exists(p);`     |
| `<bitset>`     | Compile‑time bit masks      | `std::bitset<8> b{0b10101010};`                |

---

## 8️⃣ Best Practices

1. **Prefer value semantics** – pass by value when small, otherwise `const&`.
2. **Use `auto` wisely** – avoid overuse; keep readability.
3. **Keep constructors noexcept if possible** – helps with container moves.
4. **Prefer range‑based loops and algorithms** – less error‑prone.
5. **Avoid raw pointers unless interfacing with C APIs** – use smart pointers instead.
6. **Always initialize variables** – `int x{}` or `auto x = 0;`.
7. **Leverage `constexpr` for compile‑time constants** – reduces runtime cost.
8. **Prefer `std::string_view` over `const std::string&`** when only reading.

---

## 9️⃣ Common Gotchas

| Issue                                                 | Symptom                           | Fix                                               |
| ----------------------------------------------------- | --------------------------------- | ------------------------------------------------- |
| **Copy elision disabled**                             | Extra copy/move constructor calls | Mark constructors `noexcept`; use `std::move`.    |
| **Dangling reference to temporary**                   | Segfault or UB                    | Store in a variable, or return by value.          |
| **Uninitialized data in vector**                      | Random values                     | Use `reserve()` + push_back, or `vector<T> v(n);` |
| **Implicit conversions breaking overload resolution** | Unexpected overload chosen        | Use explicit constructors /`=delete`.             |

---

## 🔧 Quick Reference Commands

```bash
# Compile with C++23 and warnings as errors
g++ -std=c++23 -Wall -Wextra -pedantic -Werror main.cpp -o app

# Run unit tests (Google Test)
ctest --output-on-failure

# Check memory usage
valgrind --leak-check=full ./app
```

---

## 📖 Further Reading

- _Effective Modern C++_ – Scott Meyers
- _C++ Concurrency in Action_ – Anthony Williams
- _The C++ Programming Language_ (latest edition) – Bjarne Stroustrup
- Official C++ reference: [cppreference.com](https://en.cppreference.com/)

---

**Happy coding!** 🚀
