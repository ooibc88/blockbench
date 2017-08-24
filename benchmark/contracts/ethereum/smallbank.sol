pragma solidity ^0.4.0;

contract SmallBank {
    
    //uint constant MAX_ACCOUNT = 10000;
    //uint constant BALANCE = 10000;
    //bytes20 constant accountTab = "account";
    //bytes20 constant savingTab = "saving";
    //bytes20 constant checkingTab = "checking";
    
    mapping(string=>uint) savingStore;
    mapping(string=>uint) checkingStore;

    function almagate(string arg0, string arg1) {
       uint bal1 = savingStore[arg0];
       uint bal2 = checkingStore[arg1];
       
       checkingStore[arg0] = 0;
       savingStore[arg1] = bal1 + bal2;
    }

    function getBalance(string arg0) constant returns (uint balance) {
        uint bal1 = savingStore[arg0];
        uint bal2 = checkingStore[arg0];
        
        balance = bal1 + bal2;
        return balance;
    }
    
    function updateBalance(string arg0, uint arg1) {
        uint bal1 = checkingStore[arg0];
        uint bal2 = arg1;
        
        checkingStore[arg0] = bal1 + bal2;
    }
    
    function updateSaving(string arg0, uint arg1) {
        uint bal1 = savingStore[arg0];
        uint bal2 = arg1;
        
        savingStore[arg0] = bal1 + bal2;
    }
    
    function sendPayment(string arg0, string arg1, uint arg2) {
        uint bal1 = checkingStore[arg0];
        uint bal2 = checkingStore[arg1];
        uint amount = arg2;
        
        bal1 -= amount;
        bal2 += amount;
        
        checkingStore[arg0] = bal1;
        checkingStore[arg1] = bal2;
    }
    
    function writeCheck(string arg0, uint arg1) {
        uint bal1 = checkingStore[arg0];
        uint bal2 = savingStore[arg0];
        uint amount = arg1;
        
        if (amount < bal1 + bal2) {
            checkingStore[arg0] = bal1 - amount - 1;
        } 
        else {
            checkingStore[arg0] = bal1 - amount;
        }
    }
    
    function uintToBytes(uint v) internal constant returns (bytes20 ret) {
        if (v == 0) {
            ret = '00000000000000000000';
        }
        else {
            while (v > 0) {
                ret = bytes20(uint(ret) / (2 ** 8));
                ret |= bytes20(((v % 10) + 48) * 2 ** (8 * 19));
                v /= 10;
            }
        }
        return ret;
    }
    
    function bytesToUint(bytes20 arg) internal constant returns (uint v) {
        for (uint i = 0; i < arg.length; i++) {
            v = v * 10 + (uint(arg[i]) - 48);
        }
        return v;
    }
}

