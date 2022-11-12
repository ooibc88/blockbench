package main

import (
	"fmt"

	"github.com/hyperledger/fabric-contract-api-go/contractapi"
)

type DoNothing struct {
	contractapi.Contract
}

func main() {
	chaincode, err := contractapi.NewChaincode(new(DoNothing))
	if err != nil {
		fmt.Printf("Error starting DoNothing: %s", err)
	}
	if err := chaincode.Start(); err != nil {
		fmt.Printf("Error starting DoNothing: %s", err.Error())
	}
}


func (t *DoNothing) Invoke(ctx contractapi.TransactionContextInterface) error {
	return nil
}
