# Rust Cheat‑Sheet

_(A quick reference you can keep on your desk or in a note‑taking app)_

---

## 1️⃣ Quick Start

```bash
# Create a new project
cargo new my_app          # creates src/main.rs + Cargo.toml

# Build & run
cargo run

# Test
cargo test

# Add dependency
echo 'serde = "1.0"' >> Cargo.toml
```

---

## 2️⃣ Core Concepts

| Concept            | What it is                    | Syntax / Example            |
| ------------------ | ----------------------------- | --------------------------- |
| **Variables**      | Immutable by default          | `let x = 5;`                |
| **Mutable**        | `mut` keyword                 | `let mut y = 10; y += 1;`   |
| **Shadowing**      | Re‑bind a name                | `let x = 5; let x = x + 2;` |
| **Type inference** | Usually inferred              | `let s = "hello";`          |
| **Explicit types** | For clarity / ambiguous cases | `let count: i32 = 0;`       |

---

## 3️⃣ Ownership & Borrowing

| Rule                       | Explanation                                        | Example                                                 |
| -------------------------- | -------------------------------------------------- | ------------------------------------------------------- |
| One owner at a time        | A value has exactly one owner.                     | `let s1 = String::from("hi"); let s2 = s1; // s1 moved` |
| Drop on scope end          | When the owner goes out of scope, memory is freed. | `{ let s = String::new(); } // dropped at brace close`  |
| References (`&`)           | Borrow without taking ownership.                   | `let r = &s;`                                           |
| Mutable reference (`&mut`) | One mutable borrow allowed.                        | `fn add_one(x: &mut i32) { *x += 1; }`                  |
| Dangling refs not allowed  | Compile‑time safety.                               | ❌                                                      |

### Lifetime syntax

```rust
fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() { x } else { y }
}
```

---

## 4️⃣ Data Types

| Category   | Primitive                           | Size / Example         |
| ---------- | ----------------------------------- | ---------------------- |
| Integers   | `i8`, `u32`                         | signed/unsigned        |
| Floats     | `f64`                               | double precision       |
| Booleans   | `bool`                              | `true` / `false`       |
| Characters | `char`                              | Unicode scalar (`'a'`) |
| Strings    | `String` (owned), `&str` (borrowed) | UTF‑8                  |

---

## 5️⃣ Control Flow

```rust
if condition { ... } else if ... {}

loop { break; }
while cond { ... }

for i in 0..10 { println!("{}", i); }

// Pattern matching
match value {
    Some(x) => println!("{}", x),
    None => (),
}
```

---

## 6️⃣ Functions & Methods

```rust
fn add(a: i32, b: i32) -> i32 { a + b }

struct Point { x: f64, y: f64 }
impl Point {
    fn new(x: f64, y: f64) -> Self { Self { x, y } }
    fn distance(&self, other: &Self) -> f64 { /*...*/ }
}
```

---

## 7️⃣ Structs

```rust
// Tuple struct
struct Color(u8, u8, u8);

// Unit-like struct
struct Marker;

// Named fields
#[derive(Debug)]
struct Person {
    name: String,
    age: u8,
}
```

---

## 8️⃣ Enums & Pattern Matching

```rust
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(u8, u8, u8),
}

let msg = Message::Write("hello".into());
match msg {
    Message::Quit => (),
    Message::Move { x, y } => println!("{} {}", x, y),
    _ => (),
}
```

---

## 9️⃣ Traits & Trait Objects

```rust
trait Drawable {
    fn draw(&self);
}

impl Drawable for Circle { /*...*/ }

fn render<T: Drawable>(obj: &T) { obj.draw(); }

// Dynamic dispatch
let shapes: Vec<Box<dyn Drawable>> = vec![Box::new(Circle), Box::new(Rectangle)];
for shape in shapes.iter() {
    shape.draw();
}
```

---

## 🔟 Error Handling

| Approach                 | Syntax                                                                 |
| ------------------------ | ---------------------------------------------------------------------- |
| `Result<T, E>` (checked) | `fn parse(s: &str) -> Result<i32, ParseIntError> { s.parse::<i32>() }` |
| `panic!` (unchecked)     | `if x < 0 { panic!("negative"); }`                                     |
| `?` operator             | `let val = some_func()?;`                                              |
| `match` on Result        | `match r { Ok(v) => ..., Err(e) => ... }`                              |

---

## 1️⃣1️⃣ Collections

```rust
// Vector
let mut v: Vec<i32> = vec![1, 2, 3];
v.push(4);

// HashMap
use std::collections::HashMap;
let mut map = HashMap::new();
map.insert("key", 42);
```

---

## 1️⃣2️⃣ Concurrency

```rust
// Threads
use std::thread;
let handle = thread::spawn(|| println!("Hello from thread!"));
handle.join().unwrap();

// Arc + Mutex for shared mutable state
use std::sync::{Arc, Mutex};
let counter = Arc::new(Mutex::new(0));
```

---

## 1️⃣3️⃣ Common Cargo Features

| Feature                                   | Command                      |
| ----------------------------------------- | ---------------------------- |
| Add dev dependency                        | `cargo add --dev serde_json` |
| Build release                             | `cargo build --release`      |
| Run tests with coverage (needs tarpaulin) | `cargo tarpaulin`            |

---

## 1️⃣4️⃣ Useful Standard Library Items

| Module      | Use‑Case                                   |
| ----------- | ------------------------------------------ |
| `std::fs`   | File I/O (`read_to_string`, `write`)       |
| `std::io`   | stdin/out, buffering                       |
| `std::path` | Path handling                              |
| `std::env`  | Environment vars, args                     |
| `std::time` | Duration, Instant                          |
| `std::fmt`  | Custom formatting with `Display` / `Debug` |

---

## 1️⃣5️⃣ Common Patterns

| Pattern     | When to use                                     | Example                                                             |
| ----------- | ----------------------------------------------- | ------------------------------------------------------------------- |
| **Builder** | Complex struct initialization                   | `let cfg = Config::builder().host("localhost").port(8080).build();` |
| **Factory** | Return different concrete types behind a trait  | `fn create(shape: &str) -> Box<dyn Drawable> { ... }`               |
| **Visitor** | Operate on enum variants without adding methods | `trait Visitor { fn visit(&self, m: &Message); }`                   |

---

## 1️⃣6️⃣ Quick Debugging

```rust
// Print value
println!("{:?}", val);          // Debug
println!("{}", val);            // Display

// Breakpoint (VS Code/CLion) – set in editor or `debugger::break();`

// Use the REPL: `evcxr`
cargo install evcxr_repl
evcxr
```

---

## 1️⃣7️⃣ Resources

| Resource                | Link                                       |
| ----------------------- | ------------------------------------------ |
| Official Book           | https://doc.rust-lang.org/book/            |
| Rust By Example         | https://doc.rust-lang.org/rust-by-example/ |
| Rust Playground         | https://play.rust-lang.org/                |
| Rust Analyzer (VS Code) | https://rust-analyzer.github.io/           |

---

### 🎉 Final Tip

> **Think in ownership** – before you write code, ask: _who owns this data?_
> If it can be borrowed, borrow. If it must outlive its scope, use `Arc` or `Box`.

Happy coding!
