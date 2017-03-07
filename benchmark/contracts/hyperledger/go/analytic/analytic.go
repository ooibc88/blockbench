package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric/core/chaincode/shim"
)

type Analytic struct {
}

type Transaction struct {
	From string
	To   string
	Val  int
}

type AccountBalance struct {
	Val        int
	StartBlock int
}

func main() {
	err := shim.Start(new(Analytic))
	if err != nil {
		fmt.Printf("Error starting analytic workload: %s", err)
	}
}

func gen_acc(acc int) string {
	zeros := "00000000000000000000"
	str := strconv.Itoa(acc)
	return "0x" + zeros[0:(20-len(str))] + str
}

func get_tip(stub shim.ChaincodeStubInterface) (int, error) {
	valAsbytes, err := stub.GetState("tip")
	if err != nil {
		return -1, errors.New("Error when get tip block: " + err.Error())
	}
	var tip int
	tip, err = strconv.Atoi(string(valAsbytes))
	if err != nil {
		return -1, errors.New("Error when get tip block: " + err.Error())
	}
	return tip, nil
}

func get_pending(stub shim.ChaincodeStubInterface) ([]Transaction, error) {
	valAsbytes, err := stub.GetState("pending")
	if err != nil {
		return nil, errors.New("Error when get pending transactions: " + err.Error())
	}
	var pending_txns []Transaction
	err = json.Unmarshal(valAsbytes, &pending_txns)
	if err != nil {
		return nil, errors.New("Error unmarshal pending transaction list: " + err.Error())
	}
	return pending_txns, nil
}

func put_pending(stub shim.ChaincodeStubInterface, key string, pending_txns []Transaction) error {
	valAsbytes, err := json.Marshal(pending_txns)
	if err != nil {
		return errors.New("Error marshalling pending transaction list: " + err.Error())
	}
	err = stub.PutState(key, valAsbytes)
	if err != nil {
		return errors.New("Error put pending transaction list: " + err.Error())
	}
	return nil
}

// 3 args
// @arg1 from account
// @arg2 to account
// @arg3 value transferred
func send(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var val int
	var err error

	if len(args) != 3 {
		return nil, errors.New("Incorrect number of arguments. Expecting 3.")
	}
	val, err = strconv.Atoi(args[2])
	if err != nil {
		return nil, errors.New("Expecting integer value for asset holding")
	}
	var pending_txns []Transaction
	pending_txns, err = get_pending(stub)
	if err != nil {
		return nil, err
	}
	txn := Transaction{
		From: args[0],
		To:   args[1],
		Val:  val,
	}
	pending_txns = append(pending_txns, txn)
	err = put_pending(stub, "pending", pending_txns)
	if err != nil {
		return nil, err
	}
	return nil, nil
}

func get_latest_version(stub shim.ChaincodeStubInterface, acc string) (int, error) {
	valAsbytes, err := stub.GetState(acc + ":latest")
	if err != nil {
		return -1, errors.New("Error fetch newest version for account: " + acc)
	}
	var version int
	version, err = strconv.Atoi(string(valAsbytes))
	if err != nil {
		return -1, errors.New("Error convert newest version for account: " + acc + " - " + string(valAsbytes))
	}
	return version, nil
}

func set_latest_version(stub shim.ChaincodeStubInterface, acc string, version int) error {
	err := stub.PutState(acc+":latest", []byte(strconv.Itoa(version)))
	if err != nil {
		return errors.New("Error put pending transaction list: " + err.Error())
	}
	return nil
}

func get_balance(stub shim.ChaincodeStubInterface, acc string, version int) (AccountBalance, error) {
	valAsbytes, err := stub.GetState(acc + ":" + strconv.Itoa(version))
	var balance AccountBalance
	err = json.Unmarshal(valAsbytes, &balance)
	if err != nil {
		return AccountBalance{}, errors.New("Error unmarshal newest balance info for account: " + acc + " - " + string(valAsbytes))
	}
	return balance, nil
}

func set_balance(stub shim.ChaincodeStubInterface, acc string, balance AccountBalance, version int) error {
	valAsbytes, err := json.Marshal(balance)
	if err != nil {
		return err
	}
	err = stub.PutState(acc+":"+strconv.Itoa(version), valAsbytes)
	if err != nil {
		return errors.New("Error put account balance: " + err.Error())
	}
	return nil
}

func change_balance(stub shim.ChaincodeStubInterface, acc string, delta int, tip int) error {
	var err error
	var version int
	version, err = get_latest_version(stub, acc)
	if err != nil {
		return err
	}
	var balance AccountBalance
	balance, err = get_balance(stub, acc, version)
	if err != nil {
		return err
	}
	balance.StartBlock = tip
	balance.Val += delta
	version++
	err = set_balance(stub, acc, balance, version)
	if err != nil {
		return err
	}
	err = set_latest_version(stub, acc, version)
	if err != nil {
		return err
	}
	return nil
}

func commit_txn(stub shim.ChaincodeStubInterface, txn Transaction, tip int) error {
	var err error
	err = change_balance(stub, txn.From, -txn.Val, tip)
	if err != nil {
		return err
	}
	err = change_balance(stub, txn.To, txn.Val, tip)
	if err != nil {
		return err
	}
	return nil
}

// no arg
// step 1. update balances
// step 2. increment tip
// step 3. store transaction list into block:tip
// step 4. clean pending list
func commit(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var err error
	var tip int
	tip, err = get_tip(stub)
	if err != nil {
		return nil, err
	}
	tip++
	var pending_txns []Transaction
	pending_txns, err = get_pending(stub)
	if err != nil {
		return nil, err
	}
	for _, txn := range pending_txns {
		err = commit_txn(stub, txn, tip)
		if err != nil {
			return nil, err
		}
	}
	err = stub.PutState("tip", []byte(strconv.Itoa(tip)))
	if err != nil {
		return nil, err
	}
	err = put_pending(stub, "block:"+strconv.Itoa(tip), pending_txns)
	if err != nil {
		return nil, err
	}
	err = put_pending(stub, "pending", pending_txns[:0])
	if err != nil {
		return nil, err
	}
	return nil, nil
}

func init_acc(stub shim.ChaincodeStubInterface, n int) error {
	balance := AccountBalance{1000000000, 0}
	acc := gen_acc(n)
	err := set_latest_version(stub, acc, 0)
	if err != nil {
		return err
	}
	err = set_balance(stub, acc, balance, 0)
	if err != nil {
		return err
	}
	return nil
}

// @arg1 initial account number
func (t *Analytic) Init(stub shim.ChaincodeStubInterface, function string, args []string) ([]byte, error) {
	num, err := strconv.Atoi(args[0])
	if err != nil {
		return nil, err
	}
	// tip = 0
	err = stub.PutState("tip", []byte(strconv.Itoa(0)))
	if err != nil {
		return nil, err
	}
	// init account (latest_version & balance)
	for i := 0; i < num; i++ {
		err = init_acc(stub, i)
		if err != nil {
			return nil, err
		}
	}
	// empty pending list
	err = put_pending(stub, "pending", make([]Transaction, 0))
	if err != nil {
		return nil, err
	}
	return nil, nil
}

func (t *Analytic) Invoke(stub shim.ChaincodeStubInterface, function string, args []string) ([]byte, error) {
	if function == "Send" {
		return send(stub, args)
	} else if function == "Commit" {
		return commit(stub, args)
	}
	return nil, errors.New("Received unknown function invocation: " + function)
}

// @return transaction list of block @arg1
func query_block(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var err error
	var tip, req int
	var valAsbytes []byte
	tip, err = get_tip(stub)
	if err != nil {
		return nil, err
	}
	req, err = strconv.Atoi(args[0])
	if err != nil {
		return nil, err
	}
	if req > tip {
		return nil, errors.New("Non-exist block")
	}
	valAsbytes, err = stub.GetState("block:" + args[0])
	if err != nil {
		return nil, errors.New("Error when get block: " + args[0])
	}
	return valAsbytes, nil
}

// @arg1 account
// @arg2 block num
func query_account(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var err error
	var tip, req_blk int
	req_acc := args[0]
	tip, err = get_tip(stub)
	if err != nil {
		return nil, err
	}
	req_blk, err = strconv.Atoi(args[1])
	if err != nil {
		return nil, errors.New("Expect integer block number")
	}
	if req_blk > tip {
		return nil, errors.New("Non-exist block")
	}
	var version int
	version, err = get_latest_version(stub, req_acc)
	if err != nil {
		return nil, err
	}
	var balance AccountBalance
	l, r := 0, version
	ret := -1
	for l <= r {
		mid := (l + r) / 2
		balance, err = get_balance(stub, req_acc, mid)
		if err != nil {
			return nil, err
		}
		if balance.StartBlock <= req_blk {
			l = mid + 1
			ret = balance.Val
		} else {
			r = mid - 1
		}
	}
	return []byte(strconv.Itoa(ret)), nil
}

// @arg1 account
// @arg2 start_block
// @arg3 end_block
func range_account(stub shim.ChaincodeStubInterface, args []string) ([]byte, error) {
	var err error
	var tip, start_blk, end_blk int
	req_acc := args[0]
	tip, err = get_tip(stub)
	if err != nil {
		return nil, err
	}
	start_blk, err = strconv.Atoi(args[1])
	if err != nil {
		return nil, errors.New("Expect integer block number")
	}
	end_blk, err = strconv.Atoi(args[2])
	if err != nil {
		return nil, errors.New("Expect integer block number")
	}
	if end_blk > tip || start_blk > end_blk {
		return nil, errors.New("Invalid block number")
	}
	var version int
	version, err = get_latest_version(stub, req_acc)
	if err != nil {
		return nil, err
	}
	var balance AccountBalance
	balance, err = get_balance(stub, req_acc, version)
	if err != nil {
		return nil, err
	}
	ret := -1
	for balance.StartBlock >= start_blk {
		version--
		balance, err = get_balance(stub, req_acc, version)
		if err != nil {
			return nil, err
		}
		if balance.Val > ret && balance.StartBlock >= start_blk && balance.StartBlock <= end_blk {
			ret = balance.Val
		}
	}
	return []byte(strconv.Itoa(ret)), nil
}

func (t *Analytic) Query(stub shim.ChaincodeStubInterface, function string, args []string) ([]byte, error) {
	if function == "QueryBlock" {
		return query_block(stub, args)
	} else if function == "QueryAccount" {
		return query_account(stub, args)
	} else if function == "RangeAccount" {
		return range_account(stub, args)
	}
	return nil, errors.New("Received unknown function query")
}
