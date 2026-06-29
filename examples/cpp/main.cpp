// ============================================================
// Modern C++ Examples  (C++20)
// ============================================================
// Compile:  g++ -std=c++20 -Wall -Wextra -pedantic -o main main.cpp
// Run:      ./main
//
// This file is a single-file, self-contained walkthrough of
// modern C++ (C++17 / C++20) idioms and standard library features.
// Each section header marks a topic; read top-to-bottom or jump
// to the section you need.
// ============================================================

#include <algorithm>      // std::sort, std::find_if, std::transform …
#include <any>            // std::any  (C++17)
#include <array>          // std::array
#include <atomic>         // std::atomic
#include <cassert>        // assert()
#include <chrono>         // std::chrono – time points & durations
#include <filesystem>     // std::filesystem  (C++17)
#include <fstream>        // std::ifstream, std::ofstream
#include <functional>     // std::function, std::invoke
#include <future>         // std::future, std::async, std::promise
#include <limits>         // std::numeric_limits
#include <iostream>       // std::cout / std::cerr
#include <map>            // std::map
#include <memory>         // std::unique_ptr, std::shared_ptr …
#include <mutex>          // std::mutex, std::lock_guard …
#include <numeric>        // std::accumulate, std::iota
#include <optional>       // std::optional  (C++17)
#include <ranges>         // std::views::filter / transform  (C++20)
#include <set>            // std::set
#include <sstream>        // std::ostringstream
#include <stdexcept>      // std::runtime_error …
#include <string>         // std::string
#include <string_view>    // std::string_view  (C++17)
#include <thread>         // std::thread
#include <unordered_map>  // std::unordered_map
#include <variant>        // std::variant, std::visit  (C++17)
#include <vector>         // std::vector

// Namespace alias – keeps filesystem paths short throughout the file.
namespace fs = std::filesystem;

// ============================================================
// SECTION 1: BASICS – TYPES, AUTO, CONST, CONSTEXPR
// ============================================================
// Key ideas:
//  • Prefer brace-initialisation {}: catches narrowing conversions.
//  • auto deduces type from the initialiser; use it to reduce noise.
//  • const  = immutable at runtime.
//  • constexpr = evaluated at compile time when possible.

// constexpr function: result computed at compile time when arguments are
// compile-time constants, otherwise computed at runtime.
constexpr int square(int n) { return n * n; }

// A compile-time constant that other code (e.g. std::array sizes) can use.
constexpr std::size_t kMaxItems{16};

void basics_examples() {
    std::cout << "\n== Section 1: Basics & Types ==\n";

    // Brace-initialisation: uniform syntax for all types.
    int count{42};
    double ratio{3.14159};
    bool active{true};
    char grade{'A'};

    // auto lets the compiler deduce type; improves maintainability when
    // types are long (e.g. iterators, lambdas).
    auto message = std::string{"Hello, Modern C++"};
    auto pi      = 3.14159265358979;  // deduced as double

    // string_view is a non-owning, cheap-to-copy view of a character sequence.
    // Prefer it over const std::string& when you only need to read.
    std::string_view greeting{"world"};

    // Structured bindings (C++17): unpack tuple/pair/array/struct members.
    auto [x, y] = std::pair{10, 20};

    // if-with-initialiser (C++17): scope the temporary to the if block only.
    if (auto half = count / 2; half > 10) {
        std::cout << "half of count is " << half << "\n";
    }

    // constexpr at call site: result is folded to 1764 by the compiler.
    constexpr int sq = square(42);

    std::cout << "count=" << count << " ratio=" << ratio
              << " active=" << active << " grade=" << grade << "\n";
    std::cout << "message=" << message << " greeting=" << greeting << "\n";
    std::cout << "structured binding x=" << x << " y=" << y << "\n";
    std::cout << "compile-time square(42)=" << sq << "\n";
    std::cout << "pi=" << pi << "\n";

    // Scoped enum (enum class): no implicit conversion to int, no name leaking.
    enum class Direction { North, South, East, West };
    auto heading = Direction::North;
    if (heading == Direction::North) {
        std::cout << "heading north\n";
    }
}

// ============================================================
// SECTION 2: OOP – CLASSES, RAII, INHERITANCE, POLYMORPHISM
// ============================================================
// C++ OOP highlights:
//  • RAII (Resource Acquisition Is Initialisation): tie resource lifetime to
//    object lifetime so cleanup is automatic via destructors.
//  • Rule of Five: if you define a destructor, copy constructor, copy
//    assignment, move constructor, or move assignment, consider defining all
//    five (or use = default / = delete).
//  • virtual + override: opt-in runtime polymorphism; mark with override to
//    catch typos at compile time.

// Abstract base class.  Pure virtual (= 0) makes Shape non-instantiable.
class Shape {
public:
    // Virtual destructor: essential for correct cleanup via base pointer.
    virtual ~Shape() = default;

    // Pure virtual interface – every concrete shape must implement this.
    [[nodiscard]] virtual double area() const = 0;

    // Non-virtual helper that uses the virtual area().
    void print_area() const {
        std::cout << "area = " << area() << "\n";
    }
};

// Concrete class – inherits from Shape.
class Circle : public Shape {
public:
    // explicit prevents accidental implicit conversions: Circle c = 3.0.
    explicit Circle(double radius) : radius_{radius} {}

    [[nodiscard]] double area() const override {
        return 3.14159265358979 * radius_ * radius_;
    }

private:
    double radius_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : width_{width}, height_{height} {}

    [[nodiscard]] double area() const override { return width_ * height_; }

private:
    double width_, height_;
};

// RAII resource wrapper: models a file handle whose lifetime is tied to the
// object.  The destructor closes the handle automatically, even on exceptions.
class FileHandle {
public:
    explicit FileHandle(const fs::path& path)
        : path_{path}, handle_{std::fopen(path.string().c_str(), "w")} {
        if (!handle_) {
            throw std::runtime_error{"could not open file: " + path.string()};
        }
    }

    // Destructor runs when the object goes out of scope – no manual cleanup.
    ~FileHandle() {
        if (handle_) {
            std::fclose(handle_);
        }
    }

    // Delete copy operations: a file handle should not be duplicated.
    FileHandle(const FileHandle&)            = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Allow move: transfer ownership without duplicating the handle.
    FileHandle(FileHandle&& other) noexcept
        : path_{std::move(other.path_)}, handle_{other.handle_} {
        other.handle_ = nullptr;
    }
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (handle_) std::fclose(handle_);
            path_         = std::move(other.path_);
            handle_       = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    void write(std::string_view text) const {
        std::fputs(text.data(), handle_);
    }

private:
    fs::path path_;
    std::FILE* handle_;
};

void oop_examples() {
    std::cout << "\n== Section 2: OOP – Classes & Inheritance ==\n";

    // Polymorphism via base pointer.
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));

    for (const auto& shape : shapes) {
        shape->print_area();
    }

    // RAII demo: FileHandle cleans itself up at end of scope.
    {
        auto tmp = fs::temp_directory_path() / "cpp-example-raii.txt";
        FileHandle fh{tmp};
        fh.write("RAII example\n");
        std::cout << "wrote file via RAII handle, will close automatically\n";
        // fh destructor runs here – file is closed.
        fs::remove(tmp);  // clean up temp file
    }
}

// ============================================================
// SECTION 3: TEMPLATES & CONCEPTS (C++20)
// ============================================================
// Templates generate code for multiple types at compile time.
// Concepts (C++20) attach named constraints to template parameters,
// replacing the less readable SFINAE / enable_if patterns.

// Concept: T must support operator+ and be constructible from int 0.
template <typename T>
concept Addable = requires(T a, T b) { a + b; };

// Function template constrained by a concept.
template <Addable T>
T add(T a, T b) { return a + b; }

// Class template: a fixed-capacity stack stored on the stack (no heap alloc).
template <typename T, std::size_t Capacity>
class FixedStack {
public:
    // Push returns false when the stack is full.
    bool push(T value) {
        if (size_ >= Capacity) return false;
        data_[size_++] = std::move(value);
        return true;
    }

    std::optional<T> pop() {
        if (size_ == 0) return std::nullopt;
        return data_[--size_];
    }

    [[nodiscard]] std::size_t size()     const { return size_; }
    [[nodiscard]] bool        empty()    const { return size_ == 0; }
    [[nodiscard]] bool        full()     const { return size_ == Capacity; }

private:
    std::array<T, Capacity> data_{};
    std::size_t             size_{0};
};

// constexpr if (C++17): branch on a type trait without runtime overhead.
// std::is_integral_v<T> is true for int, long, etc.
template <typename T>
std::string describe_type(T) {
    if constexpr (std::is_integral_v<T>) {
        return "integral";
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating-point";
    } else {
        return "other";
    }
}

// Variadic template + fold expression (C++17): sum any number of arguments.
template <typename... Args>
auto sum(Args... args) { return (args + ...); }  // unary right fold

void templates_examples() {
    std::cout << "\n== Section 3: Templates & Concepts ==\n";

    std::cout << "add<int>(3, 4) = "    << add(3, 4)        << "\n";
    std::cout << "add<double>(1.5, 2.5) = " << add(1.5, 2.5) << "\n";

    FixedStack<int, 4> stk;
    stk.push(10); stk.push(20); stk.push(30);
    std::cout << "stack size = " << stk.size() << "\n";
    if (auto val = stk.pop()) {
        std::cout << "popped = " << *val << "\n";
    }

    std::cout << "describe int:    " << describe_type(1)   << "\n";
    std::cout << "describe double: " << describe_type(1.0) << "\n";
    std::cout << "describe string: " << describe_type(std::string{}) << "\n";

    std::cout << "fold sum(1,2,3,4,5) = " << sum(1, 2, 3, 4, 5) << "\n";
}

// ============================================================
// SECTION 4: SMART POINTERS & MOVE SEMANTICS
// ============================================================
// unique_ptr: sole ownership – cannot be copied, only moved.
// shared_ptr: shared ownership – reference-counted.
// weak_ptr:   non-owning observer of a shared_ptr – breaks cycles.

struct Node {
    int value;
    // weak_ptr avoids the reference cycle that two shared_ptrs would create.
    std::weak_ptr<Node> sibling;
    explicit Node(int v) : value{v} {}
    ~Node() { std::cout << "Node(" << value << ") destroyed\n"; }
};

void smart_pointers_examples() {
    std::cout << "\n== Section 4: Smart Pointers & Move Semantics ==\n";

    // unique_ptr: make_unique is the preferred factory (no raw new).
    {
        auto p = std::make_unique<Circle>(3.0);
        std::cout << "unique_ptr circle area = " << p->area() << "\n";
        // p is destroyed here; Circle destructor runs automatically.
    }

    // shared_ptr: multiple owners; object lives until the last owner goes.
    {
        auto a = std::make_shared<Node>(1);
        auto b = std::make_shared<Node>(2);

        // Link the two nodes via weak_ptr to avoid a reference cycle.
        a->sibling = b;
        b->sibling = a;

        // Lock the weak_ptr to access the pointed-to object safely.
        if (auto sib = a->sibling.lock()) {
            std::cout << "a's sibling value = " << sib->value << "\n";
        }
        // Both Nodes are destroyed here; the weak_ptr broke the cycle.
    }

    // Move semantics: transfer ownership without copying.
    auto src = std::make_unique<std::string>("move me");
    auto dst = std::move(src);        // src is now nullptr; dst owns the string.
    std::cout << "moved string = " << *dst << "\n";
    std::cout << "src is " << (src ? "valid" : "null") << " after move\n";
}

// ============================================================
// SECTION 5: CONTAINERS & ALGORITHMS
// ============================================================
// The STL containers cover most data-structure needs.  Algorithms in
// <algorithm> and <numeric> operate on iterator ranges.  C++20 Ranges
// add lazy, pipe-style composition with views.

void containers_and_algorithms_examples() {
    std::cout << "\n== Section 5: Containers & Algorithms ==\n";

    // ----- std::vector -----
    std::vector<int> nums{5, 3, 1, 4, 2};

    // std::sort in place.
    std::ranges::sort(nums);
    std::cout << "sorted vector: ";
    for (auto n : nums) std::cout << n << " ";
    std::cout << "\n";

    // std::accumulate: fold a range into a single value.
    int total = std::accumulate(nums.begin(), nums.end(), 0);
    std::cout << "sum = " << total << "\n";

    // ----- std::array -----
    // Fixed-size, stack-allocated, size known at compile time.
    std::array<int, 5> arr{10, 20, 30, 40, 50};
    std::cout << "array[2] = " << arr[2] << "\n";

    // ----- std::map (ordered) -----
    std::map<std::string, int> word_count;
    for (std::string_view w : {"the", "cat", "sat", "on", "the", "mat"}) {
        ++word_count[std::string{w}];
    }
    // Range-based for with structured binding.
    for (const auto& [word, count] : word_count) {
        std::cout << word << ": " << count << "\n";
    }

    // ----- std::unordered_map (hash map, O(1) average) -----
    std::unordered_map<std::string, std::string> capitals{
        {"France", "Paris"}, {"Japan", "Tokyo"}, {"Brazil", "Brasilia"}};
    if (auto it = capitals.find("Japan"); it != capitals.end()) {
        std::cout << "capital of Japan = " << it->second << "\n";
    }

    // ----- C++20 Ranges (lazy pipeline) -----
    // views::filter and views::transform are lazy – no extra allocation.
    auto evens_squared =
        nums
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; });

    std::cout << "even squares: ";
    for (int v : evens_squared) std::cout << v << " ";
    std::cout << "\n";

    // std::views::iota: a lazy range of consecutive integers.
    auto first_five = std::views::iota(1, 6);
    std::cout << "iota(1,6): ";
    for (int v : first_five) std::cout << v << " ";
    std::cout << "\n";
}

// ============================================================
// SECTION 6: LAMBDAS & std::function
// ============================================================
// Lambdas are anonymous function objects.  They can capture surrounding
// variables by value [=] or by reference [&] (or individually).
// std::function<Ret(Args…)> is a type-erased callable container.

void lambdas_examples() {
    std::cout << "\n== Section 6: Lambdas & std::function ==\n";

    // Basic lambda.
    auto double_it = [](int x) { return x * 2; };
    std::cout << "double_it(7) = " << double_it(7) << "\n";

    // Capture by value: captures a copy of 'factor' at the time the lambda
    // is created; later changes to factor do not affect the lambda.
    int factor{3};
    auto multiply = [factor](int x) { return x * factor; };
    factor = 99;  // does NOT affect multiply's captured copy
    std::cout << "multiply(5) with captured factor=3: " << multiply(5) << "\n";

    // Capture by reference: lambda sees live value of 'accumulator'.
    int accumulator{0};
    auto accum = [&accumulator](int x) { accumulator += x; };
    for (int i : {1, 2, 3, 4, 5}) accum(i);
    std::cout << "accumulator after 1+2+3+4+5 = " << accumulator << "\n";

    // Generic lambda (C++14): auto parameters act like template parameters.
    auto print_twice = [](auto val) {
        std::cout << val << " " << val << "\n";
    };
    print_twice(42);
    print_twice("hello");

    // std::function: store any callable with matching signature.
    std::vector<std::function<int(int)>> pipeline{
        [](int x) { return x + 10; },
        [](int x) { return x * 2;  },
        [](int x) { return x - 3;  },
    };
    int result{5};
    for (const auto& fn : pipeline) result = fn(result);
    std::cout << "pipeline result starting from 5: " << result << "\n";

    // Higher-order helper that takes a std::function.
    auto apply_n = [](std::function<int(int)> fn, int value, int n) {
        for (int i = 0; i < n; ++i) value = fn(value);
        return value;
    };
    std::cout << "apply_n(double, 1, 8) = " << apply_n(double_it, 1, 8) << "\n";
}

// ============================================================
// SECTION 7: ERROR HANDLING
// ============================================================
// C++ offers several error-handling strategies:
//  1. Exceptions: throw/catch – good for truly exceptional conditions.
//  2. std::optional<T>: return a value or nothing (C++17).
//  3. std::variant<T, E>: return either a value or an error (C++17).
//     (C++23 adds std::expected<T,E> for this purpose.)

// Parses an integer from a string; returns nullopt on failure.
std::optional<int> parse_int(std::string_view text) {
    try {
        std::size_t pos{};
        int val = std::stoi(std::string{text}, &pos);
        if (pos != text.size()) return std::nullopt;  // trailing garbage
        return val;
    } catch (...) {
        return std::nullopt;
    }
}

// Error type for the variant example.
struct ParseError { std::string message; };

// Returns either a double or a ParseError.
// Using an epsilon-based check instead of exact equality is the correct way
// to test whether a floating-point value is effectively zero, because
// floating-point arithmetic can introduce tiny rounding errors.
std::variant<double, ParseError> safe_divide(double a, double b) {
    if (std::abs(b) < std::numeric_limits<double>::epsilon()) {
        return ParseError{"division by zero"};
    }
    return a / b;
}

void error_handling_examples() {
    std::cout << "\n== Section 7: Error Handling ==\n";

    // ----- Exceptions -----
    try {
        throw std::runtime_error{"something went wrong"};
    } catch (const std::runtime_error& e) {
        std::cout << "caught runtime_error: " << e.what() << "\n";
    } catch (const std::exception& e) {
        // Base class catch: handles any std::exception subtype.
        std::cout << "caught exception: " << e.what() << "\n";
    }

    // ----- std::optional -----
    for (std::string_view s : {"42", "abc", "7x"}) {
        if (auto val = parse_int(s)) {
            std::cout << "parsed \"" << s << "\" = " << *val << "\n";
        } else {
            std::cout << "failed to parse \"" << s << "\"\n";
        }
    }

    // ----- std::variant + std::visit -----
    auto result = safe_divide(10.0, 3.0);
    // std::visit dispatches to the right lambda based on the active type.
    std::visit([](auto&& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, double>) {
            std::cout << "10/3 = " << v << "\n";
        } else {
            std::cout << "error: " << v.message << "\n";
        }
    }, result);

    auto bad = safe_divide(5.0, 0.0);
    std::visit([](auto&& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, double>) {
            std::cout << "result = " << v << "\n";
        } else {
            std::cout << "division error: " << v.message << "\n";
        }
    }, bad);

    // ----- std::any (type-erased value, C++17) -----
    std::any box = 42;
    std::cout << "any holds int: " << std::any_cast<int>(box) << "\n";
    box = std::string{"now a string"};
    std::cout << "any holds string: " << std::any_cast<std::string>(box) << "\n";
}

// ============================================================
// SECTION 8: CONCURRENCY
// ============================================================
// C++ concurrency primitives live in <thread>, <mutex>, <atomic>, <future>.
//  • std::thread: lightweight OS thread.
//  • std::mutex / std::lock_guard: mutual exclusion.
//  • std::atomic<T>: lock-free operations on simple types.
//  • std::async / std::future: fire a task and retrieve its result later.

void concurrency_examples() {
    std::cout << "\n== Section 8: Concurrency ==\n";

    // ----- std::atomic -----
    // Atomic increments are safe from concurrent access without a lock.
    std::atomic<int> counter{0};
    {
        std::vector<std::thread> workers;
        for (int i = 0; i < 8; ++i) {
            workers.emplace_back([&counter]() {
                for (int j = 0; j < 100; ++j) ++counter;
            });
        }
        for (auto& t : workers) t.join();
    }
    std::cout << "atomic counter after 8 threads x 100: " << counter << "\n";

    // ----- std::mutex + std::lock_guard -----
    // lock_guard is an RAII wrapper: it locks on construction, unlocks on
    // destruction (even if an exception is thrown).
    std::mutex mtx;
    std::vector<int> shared_log;
    {
        std::vector<std::thread> loggers;
        for (int i = 0; i < 5; ++i) {
            loggers.emplace_back([&mtx, &shared_log, i]() {
                std::lock_guard<std::mutex> lg{mtx};
                shared_log.push_back(i);
            });
        }
        for (auto& t : loggers) t.join();
    }
    std::cout << "shared log size: " << shared_log.size() << "\n";

    // ----- std::async / std::future -----
    // std::async launches a task (possibly on a new thread) and returns a
    // future that holds the eventual result.
    auto fut = std::async(std::launch::async, []() -> int {
        // Simulate work.
        int s{0};
        for (int i = 1; i <= 100; ++i) s += i;
        return s;
    });
    // Do other work here while the async task runs…
    std::cout << "async sum 1..100 = " << fut.get() << "\n";

    // ----- std::promise / std::future (manual signalling) -----
    std::promise<std::string> promise;
    std::future<std::string> future = promise.get_future();

    std::thread producer{[&promise]() {
        promise.set_value("data from producer thread");
    }};
    std::cout << "promise delivered: " << future.get() << "\n";
    producer.join();
}

// ============================================================
// SECTION 9: STANDARD LIBRARY HELPERS
// ============================================================

void stdlib_helpers_examples() {
    std::cout << "\n== Section 9: Standard Library Helpers ==\n";

    // ----- std::chrono -----
    auto start = std::chrono::steady_clock::now();
    // Simulate a short workload.
    volatile int x{0};
    for (int i = 0; i < 100'000; ++i) x += i;
    auto elapsed = std::chrono::steady_clock::now() - start;
    auto us      = std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
    std::cout << "elapsed: " << us.count() << " µs\n";

    // ----- std::ostringstream (string building) -----
    std::ostringstream oss;
    oss << "value=" << 42 << " pi=" << 3.14;
    std::cout << "ostringstream result: " << oss.str() << "\n";

    // ----- std::filesystem -----
    fs::path tmp_path = fs::temp_directory_path() / "cpp-stdlib-demo.txt";
    {
        // Write a file.
        std::ofstream out{tmp_path};
        out << "line one\nline two\nline three\n";
    }
    // Read it back.
    {
        std::ifstream in{tmp_path};
        std::string line;
        int line_num{1};
        while (std::getline(in, line)) {
            std::cout << "  line " << line_num++ << ": " << line << "\n";
        }
    }
    std::cout << "file size: " << fs::file_size(tmp_path) << " bytes\n";
    fs::remove(tmp_path);

    // ----- std::string utilities -----
    std::string sentence{"  hello world  "};
    // find_first_not_of/find_last_not_of return std::string::npos when all
    // characters are spaces.  Guard against that before calling substr.
    auto ltrim = sentence.find_first_not_of(' ');
    auto rtrim = sentence.find_last_not_of(' ');
    if (ltrim != std::string::npos) {
        std::cout << "trimmed: \""
                  << sentence.substr(ltrim, rtrim - ltrim + 1) << "\"\n";
    } else {
        std::cout << "trimmed: \"\" (was all whitespace)\n";
    }
}

// ============================================================
// SECTION 10: APPLICATION EXAMPLE – IN-MEMORY TASK MANAGER
// ============================================================
// Models a small real-world object graph:
//  Task       – data record
//  TaskStore  – in-memory repository using a vector + unordered_map index
//  TaskApp    – service layer with business logic

struct Task {
    unsigned int id;
    std::string  title;
    bool         done{false};
};

class TaskStore {
public:
    // Add a task; auto-assigns an id.  Returns a reference to the stored Task.
    const Task& add(std::string title) {
        auto id = next_id_++;
        tasks_.push_back(Task{id, std::move(title), false});
        index_[id] = tasks_.size() - 1;
        return tasks_.back();
    }

    // Find by id using the index map (O(1) average).
    Task* find(unsigned int id) {
        auto it = index_.find(id);
        if (it == index_.end()) return nullptr;
        return &tasks_[it->second];
    }

    [[nodiscard]] const std::vector<Task>& all() const { return tasks_; }

private:
    std::vector<Task>                    tasks_;
    std::unordered_map<unsigned int, std::size_t> index_;
    unsigned int                         next_id_{1};
};

class TaskApp {
public:
    // Creates a task and returns its id.
    unsigned int create(std::string title) {
        const auto& t = store_.add(std::move(title));
        std::cout << "[TaskApp] created #" << t.id << " \"" << t.title << "\"\n";
        return t.id;
    }

    // Marks a task done.  Returns false if not found.
    bool complete(unsigned int id) {
        Task* t = store_.find(id);
        if (!t) return false;
        t->done = true;
        std::cout << "[TaskApp] completed #" << id << "\n";
        return true;
    }

    // Returns all open (not done) tasks.
    [[nodiscard]] std::vector<const Task*> open_tasks() const {
        std::vector<const Task*> result;
        for (const auto& t : store_.all()) {
            if (!t.done) result.push_back(&t);
        }
        return result;
    }

    void print_summary() const {
        auto all   = store_.all();
        auto open  = open_tasks();
        std::cout << "[TaskApp] total=" << all.size()
                  << " open=" << open.size()
                  << " done=" << (all.size() - open.size()) << "\n";
        for (const auto* t : open) {
            std::cout << "  [ ] #" << t->id << " " << t->title << "\n";
        }
    }

private:
    TaskStore store_;
};

void application_examples() {
    std::cout << "\n== Section 10: Application Example – Task Manager ==\n";

    TaskApp app;
    auto id1 = app.create("Write C++ cheat-sheet example");
    auto id2 = app.create("Add concurrency section");
    auto id3 = app.create("Verify compilation with g++ -std=c++20");

    app.complete(id1);
    app.complete(id2);
    app.print_summary();

    // Only id3 should remain open.
    auto open = app.open_tasks();
    assert(open.size() == 1);
    assert(open[0]->id == id3);
    std::cout << "assertion passed: exactly one task open\n";
}

// ============================================================
// MAIN
// ============================================================

int main() {
    basics_examples();
    oop_examples();
    templates_examples();
    smart_pointers_examples();
    containers_and_algorithms_examples();
    lambdas_examples();
    error_handling_examples();
    concurrency_examples();
    stdlib_helpers_examples();
    application_examples();

    std::cout << "\nAll sections complete.\n";
    return 0;
}
