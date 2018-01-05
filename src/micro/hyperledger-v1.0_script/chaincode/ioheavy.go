package main

import (
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric/core/chaincode/shim"
	sc "github.com/hyperledger/fabric/protos/peer"
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

type IO struct {
}

func main() {
	err := shim.Start(new(IO))
	if err != nil {
		fmt.Printf("Error starting io tester: %s", err)
	}
}

func (t *IO) Init(stub shim.ChaincodeStubInterface) sc.Response {
	return shim.Success(nil)
}

func (t *IO) Invoke(stub shim.ChaincodeStubInterface) sc.Response {
	function, args := stub.GetFunctionAndParameters()
	// write arg2 (k,v) from key arg1
	if function == "write" {
		return t.write(stub, args)
		// scan arg2 value from key arg1
	} else if function == "scan" {
		return t.scan(stub, args)
	} else if function == "read" {
		return t.read(stub, args)
	}

	return shim.Error("Invalid Smart Contract function name.")
}

func (t *IO) write(stub shim.ChaincodeStubInterface, args []string) sc.Response {
	var start_key, num int
	var err error

	if len(args) != 2 {
		return shim.Error("Incorrect number of arguments. Expecting 2.")
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
		stub.PutState(gen_key(start_key+i), []byte(gen_val(start_key+i)))
	}
	return shim.Success(nil)
}

func (t *IO) scan(stub shim.ChaincodeStubInterface, args []string) sc.Response {
	var start_key, num int
	var err error

	if len(args) != 2 {
		return shim.Error("Incorrect number of arguments. Expecting 2.")
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

func (t *IO) read(stub shim.ChaincodeStubInterface, args []string) sc.Response {
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
