package main

import (
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric-contract-api-go/contractapi"
)

func quicksort(slice []int, left int, right int) {
	i := left
	j := right
	if i == j {
		return
	}
	m := slice[left+(right-left)/2]
	for i <= j {
		for slice[i] < m {
			i += 1
		}
		for m < slice[j] {
			j -= 1
		}
		if i <= j {
			slice[i], slice[j] = slice[j], slice[i]
			i += 1
			j -= 1
		}
	}
	if left < j {
		quicksort(slice, left, j)
	}
	if j < right {
		quicksort(slice, i, right)
	}
}

type Sorter struct {
	contractapi.Contract
}

func main() {
	chaincode, err := contractapi.NewChaincode(new(Sorter))
	if err != nil {
		fmt.Printf("Error starting sorter: %s", err)
	}
	if err := chaincode.Start(); err != nil {
		fmt.Printf("Error starting sorter: %s", err.Error())
	}
}

func (t *Sorter) Compute(ctx contractapi.TransactionContextInterface, numArg string) (int, error) {
	num, err := strconv.Atoi(numArg)
	if err != nil {
		return 0, err
	}
	slice := make([]int, num+1, num+1)
	for i := 0; i < len(slice); i++ {
		slice[i] = num - i
	}
	quicksort(slice, 0, num)
	return slice[num], nil // return the greatest element
}
