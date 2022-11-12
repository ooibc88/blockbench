package main

import (
	"fmt"

	"github.com/hyperledger/fabric/core/chaincode/shim"
	pb "github.com/hyperledger/fabric/protos/peer"
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
func (t *KVStore) Init(stub shim.ChaincodeStubInterface) pb.Response {
	return shim.Success(nil)
}

func (t *KVStore) Invoke(stub shim.ChaincodeStubInterface) pb.Response {
	function, args := stub.GetFunctionAndParameters()
	if function == "write" {
		return t.write(stub, args)
	} else if function == "delete" {
		return t.del(stub, args)
	} else if function == "read" {
		return t.read(stub, args)
	}

	return shim.Error("Received unknown function invocation")
}

func (t *KVStore) write(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	var key, value string
	var err error

	if len(args) != 2 {
		return shim.Error("Incorrect number of arguments. Expecting 2. name of the key and value to set")
	}

	key = args[0]
	value = args[1]
	err = stub.PutState(key, []byte(value))
	if err != nil {
		return shim.Error(err.Error())
	}
	return shim.Success(nil)
}

func (t *KVStore) del(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	var key string
	var err error

	if len(args) != 1 {
		return shim.Error("Incorrect number of arguments. Expecting name of the key to delete")
	}

	key = args[0]
	err = stub.DelState(key)
	if err != nil {
		return shim.Error(err.Error())
	}
	return shim.Success(nil)
}

func (t *KVStore) read(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	var key, jsonResp string
	var err error

	if len(args) != 1 {
		return shim.Error("Incorrect number of arguments. Expecting name of the key to query")
	}

	key = args[0]
	valAsbytes, err := stub.GetState(key)
	if err != nil {
		jsonResp = "{\"Error\":\"Failed to get state for " + key + "\"}"
		return shim.Error(jsonResp)
	}

	return shim.Success(valAsbytes)
}
