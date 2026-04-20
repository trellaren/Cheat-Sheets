# Go (Golang) Cheat Sheet

_A quick‑reference guide for the essentials of writing and running Go code._

---

## 1️⃣ Basics

| Concept                         | Syntax / Example                                                                                                                               | Notes                                                                     |
| ------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- |
| **Package declaration**         | `package main`                                                                                                                                 | Every file must start with a package name.                                |
| **Imports**                     | `go\nimport (\n    "fmt"\n    "os"\n)\n`                                                                                                       | Use quotes; can alias:`import f "fmt"`.                                   |
| **Main function (entry point)** | `go\nfunc main() {\n    fmt.Println(\"Hello, world!\")\n}\n`                                                                                   | Only a program with package `main` and `func main()` runs.                |
| **Variables**                   | `go\nvar x int = 5      // explicit type\nvar y = "hi"        // inferred type\nz := 10             // short declaration (must be in func)\n`. | Short var (`:=`) works only inside functions and creates new variable(s). |
| **Constants**                   | `const Pi = 3.14159`                                                                                                                           | Can use `iota` for enumerations.                                          |
| **Comments**                    | `// single line` `<br>` `/* block comment */`                                                                                                  |                                                                           |

---

## 2️⃣ Control Flow

| Feature                                        | Syntax                                                                                                                                                        | Example                                                      |
| ---------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------ |
| **If / Else**                                  | `go\nif cond {\n    // true\n} else if otherCond {\n    // other\n} else {\n    // false\n}\n`                                                                |                                                              |
| **Switch (value)**                             | `go\nswitch v := x.(type) {  // type switch\ncase int:\n    fmt.Println(\"int\")\ndefault:\n    fmt.Println(\"other\")\n}\n`.                                 | Switch without `break` automatically breaks after each case. |
| **Select** (channels)                          | `go\nselect {\ncase msg := <-ch1:\n    fmt.Println(msg)\ncase ch2 <- 42: // send\n    fmt.Println(\"sent\")\ndefault:\n    fmt.Println(\"no activity\")\n}\n` | Non‑blocking select.                                         |
| **For loop** (C style)                         | `go\nfor i := 0; i < 10; i++ {\n    fmt.Println(i)\n}\n`.                                                                                                     | Infinite:`for {}`.                                           |
| **Range over slices, maps, strings, channels** | `go\nfor idx, val := range arr {\n    fmt.Println(idx, val)\n}\n`                                                                                             |                                                              |

---

## 3️⃣ Functions & Methods

| Feature                    | Syntax                                                                                                  | Notes                                        |
| -------------------------- | ------------------------------------------------------------------------------------------------------- | -------------------------------------------- |
| **Function definition**    | `func add(a, b int) int { return a + b }`                                                               | Multiple params of same type can be grouped. |
| **Variadic parameters**    | `func sum(nums ...int) int {}`                                                                          | Inside function:`nums` is a slice.           |
| **Multiple return values** | `func divMod(a, b int) (quotient, remainder int)`                                                       | Can ignore with `_`.                         |
| **Named return values**    | `go\nfunc f() (x int) {\n    x = 5\n    return // returns x\n}\n`                                       |                                              |
| **Method on a type**       | `go\ntype Person struct { Name string }\nfunc (p Person) Greet() string { return \"Hi, \" + p.Name }\n` | Receiver can be value or pointer.            |

---

## 4️⃣ Types & Interfaces

| Concept                          | Syntax / Example                                | Notes                                                                |
| -------------------------------- | ----------------------------------------------- | -------------------------------------------------------------------- |
| **Struct**                       | `go\ntype Point struct{ X, Y int }`.            | Anonymous fields for embedding.                                      |
| **Interface**                    | `go\ntype Shape interface { Area() float64 }\n` | A type implements an interface implicitly by having all its methods. |
| **Embedding (inheritance‑like)** | `go\ntype Circle struct{ Point; Radius int }`.  | Embedded fields/ methods promoted to outer type.                     |
| **Type assertions**              | `v, ok := i.(T)`                                | `ok` is true if the dynamic type matches.                            |
| **Type switches**                | See above in control flow.                      |                                                                      |

---

## 5️⃣ Pointers & Memory

| Topic                    | Syntax                                                        | Notes                                |
| ------------------------ | ------------------------------------------------------------- | ------------------------------------ |
| **Pointer declaration**  | `var p *int = &x`                                             | `&` gives address; `*` dereferences. |
| **Nil pointer**          | `var p *int` (defaults to nil)                                | Dereferencing nil panics.            |
| **Automatic allocation** | `p := new(int)`                                               | Returns a zero‑initialized pointer.  |
| **Zero values**          | All types have a default zero value (`0`, `""`, `nil`, etc.). |                                      |

---

## 6️⃣ Concurrency

| Primitive                | Syntax                                                                                                                    | Notes                                      |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------ |
| **goroutine**            | `go func(){ /* ... */ }()`                                                                                                | Runs concurrently.                         |
| **channel (unbuffered)** | `go\nch := make(chan int)\n`.                                                                                             | Send:`ch <- 1`; Receive: `<-ch`.           |
| **channel (buffered)**   | `ch := make(chan int, 10)`                                                                                                | Buffer capacity = 10.                      |
| **Close channel**        | `close(ch)`                                                                                                               | After closing, receives return zero value. |
| **Select with timeout**  | `go\nselect {\ncase v := <-ch:\n    fmt.Println(v)\ncase <-time.After(1*time.Second):\n    fmt.Println(\"timeout\")\n}\n` |                                            |

---

## 7️⃣ Standard Library Highlights

| Package         | Typical Use            | Quick Example                                        |
| --------------- | ---------------------- | ---------------------------------------------------- |
| `fmt`           | Printing, formatting   | `fmt.Printf("x=%d\n", x)`                            |
| `os`            | File/OS interaction    | `f, _ := os.Open("file.txt")`                        |
| `io`            | Readers/Writers        | `io.Copy(dst, src)`                                  |
| `bufio`         | Buffered I/O           | `scanner := bufio.NewScanner(os.Stdin)`              |
| `net/http`      | HTTP server/client     | `http.Get(url)`; `http.ListenAndServe(":8080", nil)` |
| `encoding/json` | JSON marshal/unmarshal | `json.Marshal(v)`                                    |
| `time`          | Timers, durations      | `time.Now()`, `time.Sleep(1*time.Second)`            |
| `strconv`       | String conversions     | `strconv.Atoi("123")`                                |
| `sync`          | Mutexes, WaitGroups    | `var wg sync.WaitGroup`                              |

---

## 8️⃣ Error Handling

```go
func readFile(name string) ([]byte, error) {
    data, err := os.ReadFile(name)
    if err != nil { return nil, fmt.Errorf("readFile: %w", err) }
    return data, nil
}
```

- Use `error` as the last return value.
- Wrap errors with `%w` to preserve stack.
- Check errors immediately after call.

---

## 9️⃣ Common Idioms

| Pattern               | Code                                                          |
| --------------------- | ------------------------------------------------------------- |
| **defer for cleanup** | `go\nf, _ := os.Open("file")\ndefer f.Close()\n`              |
| **range over map**    | `for k, v := range m {}`                                      |
| **init function**     | `go\nfunc init() { /* runs before main */ }\n`                |
| **panic / recover**   | `defer func(){ if r:=recover(); r!=nil{ fmt.Println(r) } }()` |

---

## 🔟 Tips & Tricks

- Use `gofmt` (or `go fmt`) to format code automatically.
- Run tests with `go test ./...`.
- Linting: `golangci-lint run`.
- Modules: `go mod init <module>`, `go get`, `go build`.
- To view documentation: `godoc -http=:6060` or use `pkg.go.dev`.

---

Happy coding! 🚀
