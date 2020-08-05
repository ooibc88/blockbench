package main

import (
	"fmt"

	"github.com/hyperledger/fabric-contract-api-go/contractapi"
)

type KVStore struct {
	contractapi.Contract
}

func (t *KVStore) Write(ctx contractapi.TransactionContextInterface, key string, value string) error{
	var err error
	err = ctx.GetStub().PutState(key, []byte(value))
	if err != nil {
		return err
	}
	return nil
}

func (t *KVStore) Del(ctx contractapi.TransactionContextInterface, key string) error {
	var err error
	err = ctx.GetStub().DelState(key)
	if err != nil {
		return err
	}
	return nil
}

func (t *KVStore) Read(ctx contractapi.TransactionContextInterface, key string) (string, error) {

	val, err := ctx.GetStub().GetState(key)
	if err != nil {
		return "", err
	}

	return string(val), nil
}

func main() {
	chaincode, err := contractapi.NewChaincode(new(KVStore))
	if err != nil {
		fmt.Printf("Error starting kv-store: %s", err)
	}
	if err := chaincode.Start(); err != nil {
		fmt.Printf("Error starting kv-store: %s", err.Error())
	}
}
