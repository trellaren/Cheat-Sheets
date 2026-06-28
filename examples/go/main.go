package main

import (
	"errors"
	"fmt"
	"sync"
)

type Greeter interface {
	Greet() string
}

type Person struct {
	Name string
	Age  int
}

func (p Person) Greet() string {
	return fmt.Sprintf("Hi, I'm %s", p.Name)
}

func divide(a, b float64) (float64, error) {
	if b == 0 {
		return 0, errors.New("division by zero")
	}
	return a / b, nil
}

func main() {
	numbers := []int{1, 2, 3, 4, 5}
	squares := map[int]int{}
	for _, n := range numbers {
		squares[n] = n * n
	}

	user := Person{Name: "Ada", Age: 27}
	fmt.Println(user.Greet(), squares)

	result, err := divide(10, 2)
	if err != nil {
		panic(err)
	}
	fmt.Println("10 / 2 =", result)

	ch := make(chan string, 2)
	var wg sync.WaitGroup
	wg.Add(2)
	go func() {
		defer wg.Done()
		ch <- "worker 1 done"
	}()
	go func() {
		defer wg.Done()
		ch <- "worker 2 done"
	}()
	wg.Wait()
	close(ch)
	for msg := range ch {
		fmt.Println(msg)
	}
}
