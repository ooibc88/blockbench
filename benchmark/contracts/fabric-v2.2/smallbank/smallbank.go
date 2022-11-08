package main

import (
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric-contract-api-go/contractapi"
)

type SmallBank struct {
	contractapi.Contract
}

var MAX_ACCOUNTS int = 100000
var BALANCE int = 100000
var accountTab string = "accounts"
var savingTab string = "saving"
var checkingTab string = "checking"

func main() {
	chaincode, err := contractapi.NewChaincode(new(SmallBank))
	if err != nil {
		fmt.Printf("Error starting smallbank: %s", err)
	}
	if err := chaincode.Start(); err != nil {
		fmt.Printf("Error starting smallbank: %s", err.Error())
	}
}

func (t *SmallBank) Query(ctx contractapi.TransactionContextInterface, key string) error {
	_, err := ctx.GetStub().GetState(key)
	if err != nil {
		return err
	}
	return nil
}

func (t *SmallBank) Almagate(ctx contractapi.TransactionContextInterface, from, to string) error {
	var bal1, bal2 int
	var err error
	bal_str1, err := ctx.GetStub().GetState(savingTab + "_" + from)
	if err != nil {
		bal_str1 = []byte(strconv.Itoa(BALANCE))
	}
	bal_str2, err := ctx.GetStub().GetState(checkingTab + "_" + to)
	if err != nil {
		bal_str2 = []byte(strconv.Itoa(BALANCE))
	}

	bal1, err = strconv.Atoi(string(bal_str1))
	if err != nil {
		bal1 = BALANCE
	}
	bal2, err = strconv.Atoi(string(bal_str2))
	if err != nil {
		bal2 = BALANCE
	}
	bal1 += bal2

	err = ctx.GetStub().PutState(checkingTab+"_"+from, []byte(strconv.Itoa(0)))

	if err != nil {
		return err
	}

	err = ctx.GetStub().PutState(savingTab+"_"+to, []byte(strconv.Itoa(bal1)))

	if err != nil {
		return err
	}
	return nil
}

func (t *SmallBank) GetBalance(ctx contractapi.TransactionContextInterface, account string) error {
	var bal1, bal2 int
	var err error
	bal_str1, err := ctx.GetStub().GetState(savingTab + "_" + account)
	if err != nil {
		bal_str1 = []byte(strconv.Itoa(BALANCE))
	}
	bal_str2, err := ctx.GetStub().GetState(checkingTab + "_" + account)
	if err != nil {
		bal_str2 = []byte(strconv.Itoa(BALANCE))
	}

	bal1, err = strconv.Atoi(string(bal_str1))
	if err != nil {
		bal1 = BALANCE
	}
	bal2, err = strconv.Atoi(string(bal_str2))
	if err != nil {
		bal2 = BALANCE
	}
	bal1 += bal2

	return nil
}

func (t *SmallBank) UpdateBalance(ctx contractapi.TransactionContextInterface, from, to string) error {
	bal_str, err2 := ctx.GetStub().GetState(checkingTab + "_" + from)
	if err2 != nil {
		bal_str = []byte(strconv.Itoa(BALANCE))
	}

	var bal1, bal2 int
	var err error
	bal1, err = strconv.Atoi(string(bal_str))
	if err != nil {
		bal1 = BALANCE
	}
	bal2, err = strconv.Atoi(to)
	if err != nil {
		return err
	}
	bal1 += bal2

	err = ctx.GetStub().PutState(checkingTab+"_"+from, []byte(strconv.Itoa(bal1)))

	if err != nil {
		return err
	}
	return nil
}

func (t *SmallBank) UpdateSaving(ctx contractapi.TransactionContextInterface, from, to string) error {
	bal_str3, err3 := ctx.GetStub().GetState(savingTab + "_" + from)
	if err3 != nil {
		bal_str3 = []byte(strconv.Itoa(BALANCE))
	}
	var bal1, bal2 int
	var err error

	bal1, err = strconv.Atoi(string(bal_str3))
	if err != nil {
		bal1 = BALANCE
	}
	bal2, err = strconv.Atoi(to)
	if err != nil {
		return err
	}
	bal1 += bal2

	err = ctx.GetStub().PutState(savingTab+"_"+from, []byte(strconv.Itoa(bal1)))

	if err != nil {
		return err
	}
	return nil
}

func (t *SmallBank) SendPayment(ctx contractapi.TransactionContextInterface, from, to, amountArg string) error {
	var bal1, bal2, amount int
	var err error

	bal_str1, err := ctx.GetStub().GetState(checkingTab + "_" + from)
	if err != nil {
		bal_str1 = []byte(strconv.Itoa(BALANCE))
	}
	bal_str2, err := ctx.GetStub().GetState(checkingTab + "_" + to)
	if err != nil {
		bal_str2 = []byte(strconv.Itoa(BALANCE))
	}
	amount, err = strconv.Atoi(amountArg)

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

	err = ctx.GetStub().PutState(checkingTab+"_"+from, []byte(strconv.Itoa(bal1)))

	if err != nil {
		return err
	}

	err = ctx.GetStub().PutState(checkingTab+"_"+to, []byte(strconv.Itoa(bal2)))

	if err != nil {
		return err
	}
	return nil
}

func (t *SmallBank) WriteCheck(ctx contractapi.TransactionContextInterface, from, amountArg string) error {
	bal_str2, err2 := ctx.GetStub().GetState(checkingTab + "_" + from)
	if err2 != nil {
		bal_str2 = []byte(strconv.Itoa(BALANCE))
	}
	bal_str3, err3 := ctx.GetStub().GetState(savingTab + "_" + from)
	if err3 != nil {
		bal_str3 = []byte(strconv.Itoa(BALANCE))
	}

	var bal1, bal2 int
	var err error
	var amount int
	bal1, err = strconv.Atoi(string(bal_str2))
	if err != nil {
		bal1 = BALANCE
	}
	bal2, err = strconv.Atoi(string(bal_str3))
	if err != nil {
		bal2 = BALANCE
	}
	amount, err = strconv.Atoi(amountArg)
	if amount < bal1+bal2 {
		err = ctx.GetStub().PutState(checkingTab+"_"+from, []byte(strconv.Itoa(bal1-amount-1)))
	} else {
		err = ctx.GetStub().PutState(checkingTab+"_"+from, []byte(strconv.Itoa(bal1-amount)))
	}

	if err != nil {
		return err
	}
	return nil
}
