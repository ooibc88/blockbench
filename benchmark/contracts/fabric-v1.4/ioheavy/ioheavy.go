package main

import (
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric/core/chaincode/shim"
	pb "github.com/hyperledger/fabric/protos/peer"
)

const ZEROS = "00000000000000000000"
const ALPHABET = "abcdefghijklmnopqrstuvwxy#$%^&*()_+[]{}|;:,./<>?`~"

func gen_key(k int) string {
	ret := strconv.Itoa(k)
	ret = ZEROS[:20-len(ret)] + ret
	return ret
}

func gen_val(k int) string {
	char_pool := ALPHABET + ALPHABET + ALPHABET
	return char_pool[(k % 50):(k%50 + 100)]
}

type IoHeavy struct {
}

func main() {
	err := shim.Start(new(IoHeavy))
	if err != nil {
		fmt.Printf("Error starting IoHeavy tester: %s", err)
	}
}

func (t *IoHeavy) Init(stub shim.ChaincodeStubInterface) pb.Response {
	// nothing to do
	return shim.Success(nil)
}

func (t *IoHeavy) Invoke(stub shim.ChaincodeStubInterface) pb.Response {

	// write arg2 (k,v) from key arg1
	functIoHeavyn, args := stub.GetFunctionAndParameters()
	if functIoHeavyn == "write" {
		return t.write(stub, args)
		// scan arg2 value from key arg1
	} else if functIoHeavyn == "read" {
		return t.read(stub, args)
	} else if functIoHeavyn == "scan" {
		return t.scan(stub, args)
	}

	return shim.Error("Received unknown functIoHeavyn invocatIoHeavyn")
}

func (t *IoHeavy) write(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	var start_key, num int
	var err error

	if len(args) != 2 {
		return shim.Error("Incorrect number of arguments. Expecting 2. name of the key and value to set")
	}

	start_key, err = strconv.Atoi(args[0])
	if err != nil {
		return shim.Error(err.Error())
	}
	num, err = strconv.Atoi(args[1])
	if err != nil {
		return shim.Error(err.Error())
	}
	for i := 0; i < num; i++ {
		err = stub.PutState(gen_key(start_key+i), []byte(gen_val(start_key+i)))
		if err != nil {
			return shim.Error(err.Error())
		}
	}
	return shim.Success(nil)
}

func (t *IoHeavy) scan(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	var start_key, num int
	var err error

	if len(args) != 2 {
		return shim.Error("Incorrect number of arguments. Expecting 2. name of the key and value to set")
	}

	start_key, err = strconv.Atoi(args[0])
	if err != nil {
		return shim.Error(err.Error())
	}
	num, err = strconv.Atoi(args[1])
	if err != nil {
		return shim.Error(err.Error())
	}
	for i := 0; i < num; i++ {
		_, err := stub.GetState(gen_key(start_key + i))
		if err != nil {
			return shim.Error(err.Error())
		}
	}
	return shim.Success(nil)
}

func (t *IoHeavy) read(stub shim.ChaincodeStubInterface, args []string) pb.Response {
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
