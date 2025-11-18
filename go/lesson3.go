package main

import "fmt"

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
	my_map := map[string]int{
		"apple":  5,
		"banana": 10,
	}
	my_map["orange"] = 15

	for k, v := range my_map {
		fmt.Printf("Frukt: %s, Price: %d\n", k, v)
	}

	var fruit string
	fmt.Print("Enter a fruit to delete: ")
	n, err := fmt.Scan(&fruit)
	if err != nil || n == 0 {
		fmt.Println("Invalid input!")
		return
	}

	val, exists := my_map[fruit]
	if !exists {
		fmt.Printf("%s does not exist\n", fruit)
		return
	}
	fmt.Printf("%s exists with value: %d\n", fruit, val)

	delete(my_map, fruit)
	fmt.Printf("After deleting %s:\n", fruit)

	for k, v := range my_map {
		fmt.Printf("Frukt: %s, Price: %d\n", k, v)
	}
}

func productDemo() {
}

func runInParallel() {
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
