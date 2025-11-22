package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

type Product struct {
	Name  string
	Price float64
}

func (p Product) info() {
	fmt.Printf("Product: %s, Price: %.2fSEK\n", p.Name, p.Price)
}

func (p *Product) discount(percent float64) {
	if percent < 0 || percent > 100 {
		fmt.Println("Invalid discount percent")
		return
	}
	p.Price *= (100 - percent) / 100
}

func sliceDemo() {
	slice := []int{10, 20, 30}
	slice = append(slice, 40, 50)

	for i, v := range slice {
		fmt.Printf("Index: %d, Value: %d\n", i, v)
	}
	fmt.Printf("Slice length: %d, capacity: %d\n", len(slice), cap(slice))
	slice = append(slice, 40, 50)
	fmt.Printf("After appending more elements - Slice length: %d, capacity: %d\n", len(slice), cap(slice))
}

func mapDemo() {
	var name string
	my_map := map[string]int{
		"Alice":   5,
		"Bob":     10,
		"Charlie": 20,
	}
	my_map["Dario"] = 15

	for k, v := range my_map {
		fmt.Printf("Name: %s, Age: %d\n", k, v)
	}

	fmt.Print("Enter a Name to delete: ")
	n, err := fmt.Scan(&name)
	if err != nil || n == 0 {
		fmt.Println("Invalid input!")
		return
	}

	val, exists := my_map[name]
	if !exists {
		fmt.Printf("%s does not exist\n", name)
		return
	}
	fmt.Printf("%s exists with age: %d\n", name, val)

	delete(my_map, name)
	fmt.Printf("After deleting %s:\n", name)

	for k, v := range my_map {
		fmt.Printf("Name: %s, Age: %d\n", k, v)
	}
}

func productDemo() {
	var product string
	var discount float64
	products := []Product{
		{"Laptop", 15000.0},
		{"Smartphone", 8000.0},
		{"Tablet", 5000.0},
		{"Pen", 10.0},
	}

	for i := range products {
		products[i].info()
	}

	fmt.Print("Enter product name and discount amount for Black Friday: ")
	n, err := fmt.Scanf("%s %f", &product, &discount)
	if err != nil || n != 2 {
		fmt.Println("Invalid input!")
		return
	}

	for i := range products {
		if products[i].Name == product {
			products[i].discount(discount)
			fmt.Printf("After %.2f%% discount:\n", discount)
			products[i].info()
			return
		}
	}
	fmt.Printf("Product %s not found\n", product)
}

func runInParallel() {
	var wg sync.WaitGroup
	num_tasks := 3

	wg.Add(num_tasks)
	for i := range num_tasks {
		go func(id int) {
			defer wg.Done()
			d := time.Duration(rand.Intn(1000)) * time.Millisecond
			fmt.Printf("Task %d is running for %d sec\n", id, d.Milliseconds())
			time.Sleep(d)
		}(i)
	}
	wg.Wait()
	fmt.Println("All goroutines finished")
}

func lesson3() {
	fmt.Println("lesson 3 started")

	fmt.Println("--------------------")
	fmt.Println("slice demo:")
	sliceDemo()

	fmt.Println("--------------------")
	fmt.Println("map demo:")
	mapDemo()

	fmt.Println("--------------------")
	fmt.Println("product demo:")
	productDemo()

	fmt.Println("--------------------")
	fmt.Println("run in parallel:")
	runInParallel()

	fmt.Println("lesson 3 ended")
}
