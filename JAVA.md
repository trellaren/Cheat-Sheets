# Java Cheat Sheet – Quick Reference

> **Version:** Java 21 (JDK 21)
> **Audience:** Beginners to intermediate developers
> **Format:** Compact, table‑style notes that fit on a single A4 page

---

## 1️⃣ Language Basics

| Feature      | Syntax / Example                                        | Notes                                                       |
| ------------ | ------------------------------------------------------- | ----------------------------------------------------------- |
| **Package**  | `package com.example;`                                  | Declares namespace. Must be first non-comment line.         |
| **Import**   | `import java.util.*;`                                   | Braces for static import:`import static java.lang.Math.PI;` |
| **Class**    | `java public class MyApp {}`                            | `public` only allowed in one file per `.java`.              |
| **Main**     | `public static void main(String[] args) {}`             | Entry point.                                                |
| **Comments** | `// single line` `<br>/* block */` `<br>/** javadoc */` | Javadoc used for API docs.                                  |

---

## 2️⃣ Data Types

| Primitive | Size (bits)   | Range / Example     |
| --------- | ------------- | ------------------- |
| `byte`    | 8             | -128 → 127          |
| `short`   | 16            | -32 768 → 32 767    |
| `int`     | 32            | -2³¹ → 2³¹‑1        |
| `long`    | 64            | -2⁶³ → 2⁶³‑1        |
| `float`   | 32 (IEEE‑754) | ~7‑digit precision  |
| `double`  | 64 (IEEE‑754) | ~15‑digit precision |
| `char`    | 16            | Unicode code unit   |
| `boolean` | —             | `true` / `false`    |

| Reference | Example                                                |
| --------- | ------------------------------------------------------ |
| `String`  | `"Hello"` (immutable, use `StringBuilder` for mutable) |
| `Object`  | Base of all classes                                    |
| Arrays    | `int[] a = {1,2,3};`                                   |
| Enums     | `enum Day { MON,TUE,… }`                               |

---

## 3️⃣ Operators

| Category        | Operator              | Description                                   | Example                      |
| --------------- | --------------------- | --------------------------------------------- | ---------------------------- |
| **Arithmetic**  | `+ - * / %`           | Basic math                                    | `int x = (a + b) * c;`       |
| **Unary**       | `++ -- + - ~ !`       | Increment/decrement, bitwise NOT, logical NOT | `i++; !flag;`                |
| **Relational**  | `< <= > >= == !=`     | Comparison                                    | `if (x < 10)`                |
| **Logical**     | `&&                   |                                               | !`                           |
| **Bitwise**     | `&                    | ^ << >> >>>`                                  | Bit manipulation             |
| **Assignment**  | `= += -= \*= /= %= &= | = ^= <<= >>= >>>=`                            | Set/modify value             |
| **Conditional** | `?:`                  | Ternary expression                            | `int max = (a > b) ? a : b;` |

---

## 4️⃣ Control Flow

```java
// if / else
if (cond) { … } else { … }

// switch (enum or int)
switch (value) {
    case 1: …
    default: …
}

// loops
for (int i = 0; i < n; i++) { … }
while (cond) { … }
do { … } while (cond);

// break / continue
break;   // exit loop/switch
continue; // skip to next iteration

// try‑catch‑finally
try {
    …
} catch (IOException e) {
    …
} finally {
    close();
}
```

---

## 4️⃣ Object‑Oriented Core

| Concept           | Key Syntax / API                                            |
| ----------------- | ----------------------------------------------------------- |
| **Inheritance**   | `class Dog extends Animal {}`                               |
| **Polymorphism**  | Method overriding, interface implementation                 |
| **Encapsulation** | Access modifiers (`private`, `protected`) + getters/setters |
| **Abstraction**   | `abstract class Shape { abstract double area(); }`          |
| **Interfaces**    | `interface Flyable { void fly(); }`                         |
| **Annotations**   | `@Override @Deprecated`                                     |

---

## 5️⃣ Collections Framework

| Collection Type | Interface         | Common Implementation                     | Notes            |
| --------------- | ----------------- | ----------------------------------------- | ---------------- |
| **List**        | `java.util.List`  | `ArrayList`, `LinkedList`                 | Ordered, indexed |
| **Set**         | `java.util.Set`   | `HashSet`, `TreeSet`                      | No duplicates    |
| **Queue**       | `java.util.Queue` | `LinkedList`, `PriorityQueue`             | FIFO / priority  |
| **Map**         | `java.util.Map`   | `HashMap`, `TreeMap`, `ConcurrentHashMap` | Key‑value pairs  |

```java
// Example
List<String> names = new ArrayList<>(Arrays.asList("Alice","Bob"));
Set<Integer> ids   = new HashSet<>(Arrays.asList(1,2,3));
Map<String,Integer> map = new HashMap<>();
map.put("age", 30);
```

---

## 6️⃣ Generics

```java
class Box<T> {
    private T value;
    public void set(T v) { value = v; }
    public T get() { return value; }
}

List<String> list = new ArrayList<>(); // diamond operator infers <String>
```

- **Wildcards:** `?`, `<? extends Number>`, `<? super Integer>`
- **Bounded type parameters**: `<T extends Number & Serializable>`

---

## 7️⃣ Exceptions

| Exception Type                                                     | When to Use                             |
| ------------------------------------------------------------------ | --------------------------------------- |
| **Checked** (`IOException`, `SQLException`)                        | Must be declared or caught.             |
| **Unchecked** (`NullPointerException`, `IllegalArgumentException`) | Runtime; no need for declaration.       |
| **Custom**                                                         | `class MyError extends Exception { … }` |

```java
try {
    // risky code
} catch (IOException e) {
    log(e);
}
```

---

## 8️⃣ Input / Output

| API                                         | Usage                                                                |
| ------------------------------------------- | -------------------------------------------------------------------- |
| **Scanner** (`java.util.Scanner`)           | Console input:`new Scanner(System.in).nextLine();`                   |
| **Files** (`java.nio.file.Files`)           | Read/write text or binary:`String content = Files.readString(path);` |
| **Streams** (`InputStream`, `OutputStream`) | Binary I/O, often wrapped in `Buffered*`.                            |
| **PrintWriter**                             | `new PrintWriter(System.out)` for formatted output.                  |

---

## 9️⃣ Concurrency Basics

| Feature                    | Key Classes / Methods                                     |
| -------------------------- | --------------------------------------------------------- |
| Threads                    | `Thread t = new Thread(runnable); t.start();`             |
| Synchronization            | `synchronized` block/method, `ReentrantLock`, `Semaphore` |
| Volatile                   | `volatile int counter;` (visibility guarantee)            |
| Executor Service           | `ExecutorService exec = Executors.newFixedThreadPool(4);` |
| Future / CompletableFuture | Asynchronous tasks                                        |

---

## 🔟 Java 8+ Features

| Feature                     | Syntax                                                        | Use‑Case                                         |
| --------------------------- | ------------------------------------------------------------- | ------------------------------------------------ |
| **Lambda**                  | `(args) -> expr`                                              | Functional interfaces (`Runnable`, `Comparator`) |
| **Method Reference**        | `ClassName::method`                                           | Cleaner lambda syntax                            |
| **Streams API**             | `.stream().filter(...).map(...).collect(Collectors.toList())` | Declarative collection processing                |
| **Optional**                | `Optional.ofNullable(obj)`                                    | Avoid null checks                                |
| **Date/Time** (`java.time`) | `LocalDate.now()`, `ZonedDateTime.of(...)`                    | Modern, immutable time API                       |
| **Records**                 | `record Point(int x, int y) {}`                               | Immutable data carriers (Java 16+)               |

---

## 📚 Quick Coding Checklist

1. **Compile:** `javac MyApp.java`
2. **Run:** `java com.example.MyApp`
3. **Package JAR:** `jar --create --file app.jar -C out/ .`

---

### 🎯 Tips for Success

- Use _IntelliJ IDEA_ or _Eclipse_ – both have powerful code completion.
- Keep your class names singular (`User`, not `Users`).
- Prefer interfaces over abstract classes when you need multiple inheritance of type.
- For mutable collections, consider `ConcurrentHashMap` if accessed by many threads.

---

**Print this page**, keep it handy while coding, and refer back whenever a syntax question pops up!
