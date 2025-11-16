package main

import "fmt"

func task1() {
	name := "Slava"
	age := 37
	var height float64 = 1.78
	fmt.Printf("Name: %s, Age: %d, Height: %.2f meters\n", name, age, height)
}

func task2(arr []int) {
	for _, v := range arr {
		fmt.Printf("%d squared = %d\n", v, v*v)
	}
}

func task3() {
	for i := 1; i <= 20; i++ {
		if i%2 == 0 {
			fmt.Printf("%d ", i)
		}
	}
	fmt.Println()
}

func task4() {
	var age int

	if _, err := fmt.Scan(&age); err != nil {
		fmt.Println("Invalid input!")
		return
	}

	if age < 0 || age > 150 {
		fmt.Printf("invalid age %d\n", age)
	} else if age < 18 {
		fmt.Println("minor")
	} else {
		fmt.Println("adult")
	}
}

func lesson2() {
	fmt.Println("lesson 2 started")

	fmt.Println("--------------------")
	fmt.Println("Task 1:")
	task1()

	fmt.Println("--------------------")
	fmt.Println("Task 2:")
	task2([]int{1, 200, 0})

	fmt.Println("--------------------")
	fmt.Println("Task 3:")
	task3()

	fmt.Println("--------------------")
	fmt.Println("Task 4:")
	task4()

	fmt.Println("lesson 2 ended")
}
