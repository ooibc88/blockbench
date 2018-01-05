package main

import (
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric/core/chaincode/shim"
	sc "github.com/hyperledger/fabric/protos/peer"
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
}

func main() {
	err := shim.Start(new(Sorter))
	if err != nil {
		fmt.Printf("Error starting Sorter tester: %s", err)
	}
}

func (t *Sorter) Init(stub shim.ChaincodeStubInterface) sc.Response {
	return shim.Success(nil)
}

func (t *Sorter) Invoke(stub shim.ChaincodeStubInterface) sc.Response {
	function, args := stub.GetFunctionAndParameters()
	if function == "sort" {
		num, err := strconv.Atoi(args[0])
		if err != nil {
			return nil, err
		}
		slice := make([]int, num+1, num+1)
		for i := 0; i < len(slice); i++ {
			slice[i] = num - i
		}
		quicksort(slice, 0, num)
		stub.PutState("num", []byte(args[0]))
		return shim.Success(nil)
	}
	return shim.Error("Invalid Smart Contract function name.")
}