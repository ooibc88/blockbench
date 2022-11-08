package main

import (
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric-contract-api-go/contractapi"
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
	contractapi.Contract
}

func main() {
	chaincode, err := contractapi.NewChaincode(new(IoHeavy))
	if err != nil {
		fmt.Printf("Error starting IoHeavy: %s", err)
	}
	if err := chaincode.Start(); err != nil {
		fmt.Printf("Error starting IoHeavy: %s", err.Error())
	}
}


func (t *IoHeavy) Write(ctx contractapi.TransactionContextInterface, start_key_arg, num_arg string) error {
	var start_key, num int
	var err error

	start_key, err = strconv.Atoi(start_key_arg)
	if err != nil {
		return err
	}
	num, err = strconv.Atoi(num_arg)
	if err != nil {
		return err
	}
	for i := 0; i < num; i++ {
		err = ctx.GetStub().PutState(gen_key(start_key+i), []byte(gen_val(start_key+i)))
		if err != nil {
			return err
		}
	}
	return nil
}

func (t *IoHeavy) Scan(ctx contractapi.TransactionContextInterface, start_key_arg, num_arg string) error {
	var start_key, num int
	var err error

	start_key, err = strconv.Atoi(start_key_arg)
	if err != nil {
		return err
	}
	num, err = strconv.Atoi(num_arg)
	if err != nil {
		return err
	}
	for i := 0; i < num; i++ {
		_, err := ctx.GetStub().GetState(gen_key(start_key + i))
		if err != nil {
			return err
		}
	}
	return nil
}

func (t *IoHeavy) Read(ctx contractapi.TransactionContextInterface, key string) ([]byte, error) {
	var jsonResp string
	var err error

	valAsbytes, err := ctx.GetStub().GetState(key)
	if err != nil {
		jsonResp = "{\"Error\":\"Failed to get state for " + key + "\"}"
		return []byte(jsonResp), err
	}

	return valAsbytes, nil
}
