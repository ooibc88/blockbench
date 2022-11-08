package main

import (
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric/core/chaincode/shim"
	pb "github.com/hyperledger/fabric/protos/peer"
)

type Analytic struct {
}

var BALANCE int = 100000

func main() {
	err := shim.Start(new(Analytic))
	if err != nil {
		fmt.Printf("Error starting Analytic: %s", err)
	}
}

func (t *Analytic) Init(stub shim.ChaincodeStubInterface) pb.Response {
	// nothing to do
	return shim.Success(nil)
}

func (t *Analytic) Invoke(stub shim.ChaincodeStubInterface) pb.Response {
	function, args := stub.GetFunctionAndParameters()

	if function == "sendPayment" {
		t.sendPayment(stub, args)
	} else {
		return shim.Error("Unrecognized function " + function)
	}

	return shim.Success(nil)
}

func (t *Analytic) sendPayment(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	var bal1, bal2, amount int
	var err error

	bal_str1, err := stub.GetState(args[0])
	if err != nil {
		bal_str1 = []byte(strconv.Itoa(BALANCE))
	}
	bal_str2, err := stub.GetState(args[1])
	if err != nil {
		bal_str2 = []byte(strconv.Itoa(BALANCE))
	}
	amount, err = strconv.Atoi(args[2])

	bal1, err = strconv.Atoi(string(bal_str1))
	if err != nil {
		bal1 = BALANCE
	}
	bal2, err = strconv.Atoi(string(bal_str2))
	if err != nil {
		bal2 = BALANCE
	}
	bal1 -= amount
	bal2 += amount

	err = stub.PutState(args[0], []byte(strconv.Itoa(bal1)))

	if err != nil {
		return shim.Error(err.Error())
	}

	err = stub.PutState(args[1], []byte(strconv.Itoa(bal2)))

	if err != nil {
		return shim.Error(err.Error())
	}

	err = stub.PutState("Value", []byte(args[2]))

	if err != nil {
		return shim.Error(err.Error())
	}
	return shim.Success(nil)
}
