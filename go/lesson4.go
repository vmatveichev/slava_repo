package main

import (
	"fmt"
)

func lesson4() {
	fmt.Println("lesson 4 started")

	fmt.Println("--------------------")
	fmt.Println("task 3:")
	num := 5
	ch := make(chan string, num)
	go func() {
		for i := range num {
			ch <- fmt.Sprintf("%d in channel", i+1)
		}
		close(ch)
	}()
	for v := range ch {
		fmt.Println(v)
	}

	fmt.Println("--------------------")
	fmt.Println("task 4:")
	ch1 := make(chan string)
	ch2 := make(chan string)
	go func() {
		ch1 <- "test 1"
	}()
	go func() {
		ch2 <- "test 2"
	}()
	select {
	case x := <-ch1:
		fmt.Println("ch1:", x)
	case y := <-ch2:
		fmt.Println("ch2:", y)
	}

	fmt.Println("lesson 4 ended")
}
