package main

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"os"
	"sort"
	"strings"
	"sync"
	"time"
)

// ============================================================
// PAGE 1: BASICS, TYPES, CONSTANTS, AND FUNCTIONS
// ============================================================

// Typed constants can be grouped.
const (
	appName = "Go Example Tour"
	version = "1.0"
)

// Sentinel errors are commonly exposed as package-level variables.
var ErrDivideByZero = errors.New("division by zero")

// Interfaces describe behavior.
type Greeter interface {
	Greet() string
}

// Structs are custom data types.
type Person struct {
	Name string `json:"name"`
	Age  int    `json:"age"`
}

// Methods attach behavior to structs.
func (p Person) Greet() string {
	return fmt.Sprintf("Hi, I'm %s and I'm %d years old.", p.Name, p.Age)
}

func divide(a, b float64) (float64, error) {
	if b == 0 {
		return 0, ErrDivideByZero
	}
	return a / b, nil
}

// Generic helper (Go 1.18+): map a slice into a new slice.
func mapSlice[T any, U any](items []T, transform func(T) U) []U {
	out := make([]U, 0, len(items))
	for _, item := range items {
		out = append(out, transform(item))
	}
	return out
}

func basicsPage() {
	fmt.Println("== PAGE 1: Basics ==")
	fmt.Println(appName, "v"+version)

	// Variables can use explicit types or type inference.
	var explicit int = 7
	inferred := 5
	total := explicit + inferred
	fmt.Println("explicit + inferred =", total)

	// Conditions and loops are straightforward and C-like.
	for i := 1; i <= 3; i++ {
		if i%2 == 0 {
			fmt.Println("even:", i)
		} else {
			fmt.Println("odd:", i)
		}
	}
}

// ============================================================
// PAGE 2: COLLECTIONS, STRUCTS, INTERFACES, GENERICS
// ============================================================

func collectionsAndTypesPage() {
	fmt.Println("\n== PAGE 2: Collections + Types ==")

	// Slices are dynamic views over arrays.
	numbers := []int{1, 2, 3, 4, 5}
	squares := mapSlice(numbers, func(n int) int { return n * n })
	fmt.Println("squares:", squares)

	// Maps are key/value collections.
	ages := map[string]int{"Ada": 27, "Grace": 35}
	ages["Linus"] = 24

	// To print map data deterministically, sort keys first.
	names := make([]string, 0, len(ages))
	for name := range ages {
		names = append(names, name)
	}
	sort.Strings(names)
	for _, name := range names {
		fmt.Printf("%s -> %d\n", name, ages[name])
	}

	// Interfaces allow polymorphism.
	var g Greeter = Person{Name: "Ada", Age: 27}
	fmt.Println("greeter says:", g.Greet())
}

// ============================================================
// PAGE 3: ERROR HANDLING, DEFER, PANIC/RECOVER
// ============================================================

func errorHandlingPage() {
	fmt.Println("\n== PAGE 3: Errors + defer ==")

	// Go favors explicit error returns.
	result, err := divide(10, 2)
	if err != nil {
		fmt.Println("unexpected error:", err)
	} else {
		fmt.Println("10 / 2 =", result)
	}

	_, err = divide(10, 0)
	if err != nil {
		// errors.Is is idiomatic for sentinel comparison.
		if errors.Is(err, ErrDivideByZero) {
			fmt.Println("caught expected error:", err)
		}
	}

	// defer runs when the surrounding function returns.
	defer fmt.Println("defer runs last in this function")
	fmt.Println("defer runs after this line")
}

// ============================================================
// PAGE 4: JSON + STANDARD LIBRARY + FILE I/O
// ============================================================

func jsonAndFilePage() error {
	fmt.Println("\n== PAGE 4: JSON + File I/O ==")

	// encoding/json uses struct tags for key names.
	user := Person{Name: "Ada", Age: 27}
	encoded, err := json.Marshal(user)
	if err != nil {
		return err
	}
	fmt.Println("json:", string(encoded))

	// os.CreateTemp creates a temporary file safely.
	tmpFile, err := os.CreateTemp("", "go-example-*.txt")
	if err != nil {
		return err
	}
	defer os.Remove(tmpFile.Name())
	defer tmpFile.Close()

	line := "temporary file line from standard library example\n"
	if _, err := tmpFile.WriteString(line); err != nil {
		return err
	}

	data, err := os.ReadFile(tmpFile.Name())
	if err != nil {
		return err
	}
	fmt.Println("read file:", strings.TrimSpace(string(data)))
	return nil
}

// ============================================================
// PAGE 5: CONCURRENCY (GOROUTINES, CHANNELS, CONTEXT, WAITGROUP)
// ============================================================

func concurrencyPage() {
	fmt.Println("\n== PAGE 5: Concurrency ==")

	// Context controls cancellation/timeouts across goroutines.
	ctx, cancel := context.WithTimeout(context.Background(), 2*time.Second)
	defer cancel()

	jobs := []int{1, 2, 3, 4, 5}
	results := make(chan string, len(jobs))
	var wg sync.WaitGroup

	for _, job := range jobs {
		wg.Add(1)
		go func(n int) {
			defer wg.Done()
			select {
			case <-ctx.Done():
				results <- fmt.Sprintf("job %d cancelled", n)
			case <-time.After(20 * time.Millisecond):
				results <- fmt.Sprintf("job %d => %d", n, n*n)
			}
		}(job)
	}

	wg.Wait()
	close(results)

	// Goroutine completion order is non-deterministic.
	// We sort for stable demo output.
	lines := make([]string, 0, len(jobs))
	for line := range results {
		lines = append(lines, line)
	}
	sort.Strings(lines)
	for _, line := range lines {
		fmt.Println(line)
	}
}

func main() {
	basicsPage()
	collectionsAndTypesPage()
	errorHandlingPage()
	if err := jsonAndFilePage(); err != nil {
		fmt.Println("json/file error:", err)
	}
	concurrencyPage()
}
