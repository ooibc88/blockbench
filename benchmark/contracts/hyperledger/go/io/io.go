package main

import (
	"errors"
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric/core/chaincode/shim"
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

// Init the kv-store
func (t *IO) Init(stub shim.ChaincodeStubInterface, function string, args []string) ([]byte, error) {
	return nil, nil
}

func (t *IO) Invoke(stub shim.ChaincodeStubInterface, function string, args []string) ([]byte, error) {

	// write arg2 (k,v) from key arg1
	if function == "write" {
		return t.write(stub, args)
		// scan arg2 value from key arg1
	} else if function == "scan" {
		return t.scan(stub, args)
	}

	return nil, errors.New("Received unknown function invocation")
}

func (t *IO) Query(stub shim.ChaincodeStubInterface, function string, args []string) ([]byte, error) {
	fmt.Println("query is running " + function)

	if function == "read" {
		return t.read(stub, args)
	}

	return nil, errors.New("Received unknown function query")
}

func (t *IO) write(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var start_key, num int
	var err error

	if len(args) != 2 {
		return nil, errors.New("Incorrect number of arguments. Expecting 2. name of the key and value to set")
	}

	start_key, err = strconv.Atoi(args[0])
	if err != nil {
		return nil, err
	}
	num, err = strconv.Atoi(args[1])
	if err != nil {
		return nil, err
	}
	for i := 0; i < num; i++ {
		err = stub.PutState(gen_key(start_key+i), []byte(gen_val(start_key+i)))
		if err != nil {
			return nil, err
		}
	}
	return nil, nil
}

func (t *IO) scan(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var start_key, num int
	var err error

	if len(args) != 2 {
		return nil, errors.New("Incorrect number of arguments. Expecting 2. name of the key and value to set")
	}

	start_key, err = strconv.Atoi(args[0])
	if err != nil {
		return nil, err
	}
	num, err = strconv.Atoi(args[1])
	if err != nil {
		return nil, err
	}
	for i := 0; i < num; i++ {
		_, err := stub.GetState(gen_key(start_key + i))
		if err != nil {
			return nil, err
		}
	}
	return nil, nil
}

func (t *IO) read(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
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
