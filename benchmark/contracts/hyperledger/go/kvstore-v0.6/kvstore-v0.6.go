package main

import (
	"errors"
	"fmt"

	"github.com/hyperledger/fabric/core/chaincode/shim"
)

type KVStore struct {
}

func main() {
	err := shim.Start(new(KVStore))
	if err != nil {
		fmt.Printf("Error starting kv-store: %s", err)
	}
}

// Init the kv-store
func (t *KVStore) Init(stub shim.ChaincodeStubInterface, function string, args []string) ([]byte, error) {
	return nil, nil
}

func (t *KVStore) Invoke(stub shim.ChaincodeStubInterface, function string, args []string) ([]byte, error) {

	if function == "write" {
		return t.write(stub, args)
	} else if function == "delete" {
		return t.del(stub, args)
	}

	return nil, errors.New("Received unknown function invocation")
}

func (t *KVStore) Query(stub shim.ChaincodeStubInterface, function string, args []string) ([]byte, error) {
	fmt.Println("query is running " + function)

	if function == "read" {
		return t.read(stub, args)
	}

	return nil, errors.New("Received unknown function query")
}

func (t *KVStore) write(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var key, value string
	var err error

	if len(args) != 2 {
		return nil, errors.New("Incorrect number of arguments. Expecting 2. name of the key and value to set")
	}

	key = args[0]
	value = args[1]
	err = stub.PutState(key, []byte(value))
	if err != nil {
		return nil, err
	}
	return nil, nil
}

func (t *KVStore) del(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var key string
	var err error

	if len(args) != 1 {
		return nil, errors.New("Incorrect number of arguments. Expecting name of the key to delete")
	}

	key = args[0]
	err = stub.DelState(key)
	if err != nil {
		return nil, err
	}
	return nil, nil
}

func (t *KVStore) read(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var key, jsonResp string
	var err error

	if len(args) != 1 {
		return nil, errors.New("Incorrect number of arguments. Expecting name of the key to query")
	}

	key = args[0]
	valAsbytes, err := stub.GetState(key)
	if err != nil {
		jsonResp = "{\"Error\":\"Failed to get state for " + key + "\"}"
		return nil, errors.New(jsonResp)
	}

	return valAsbytes, nil
}
