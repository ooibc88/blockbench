package main

import (
	"fmt"

	"github.com/hyperledger/fabric/core/chaincode/shim"
	pb "github.com/hyperledger/fabric/protos/peer"
)

type DoNothing struct {
}

func main() {
	err := shim.Start(new(DoNothing))
	if err != nil {
		fmt.Printf("Error starting donothing: %s", err)
	}
}

func (t *DoNothing) Init(stub shim.ChaincodeStubInterface) pb.Response {
	// nothing to do
	return shim.Success(nil)
}

func (t *DoNothing) Invoke(stub shim.ChaincodeStubInterface) pb.Response {
	return shim.Success(nil)
}
