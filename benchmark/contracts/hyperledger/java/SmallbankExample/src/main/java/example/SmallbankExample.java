/*
Copyright DTCC 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package example;

import org.hyperledger.java.shim.ChaincodeBase;
import org.hyperledger.java.shim.ChaincodeStub;

public class SmallbankExample extends ChaincodeBase {

  private static final String accountTab="accounts"; 
  private static final String savingTab="saving";
  private static final String checkingTab="checking";
  private static final int MAX_ACCOUNTS = 100000; 
  private static final int BALANCE = 1000000; 
	@Override
	public String run(ChaincodeStub stub, String function, String[] args) {
		switch (function) {
    case "init":
      load(stub); 
      break;
		case "amalgate":
			almagate(stub, args);
			break;
		case "getBalance":
      getBalance(stub, args);
			break;
    case "updateBalance":
      updateBalance(stub, args); 
      break; 
    case "updateSaving":
      updateSaving(stub, args); 
      break; 
    case "sendPayment":
      sendPayment(stub, args); 
      break; 
    case "writeCheck":
      writeCheck(stub, args); 
      break; 
    default:
      break; 
		}
		return null;
	}

  private void load(ChaincodeStub stub){
    for (int i=0; i<MAX_ACCOUNTS; i++){
      //load accounts
      stub.putState(accountTab+"_"+i, "name_"+i); 
      //load saving
      stub.putState(savingTab+"_"+i+"", BALANCE+""); 
      //load checking
      stub.putState(checkingTab+"_"+i, BALANCE+""); 
    }
  }

  // args: acc1, acc2
  private void almagate(ChaincodeStub stub, String[] args){
    // get customer ID
    stub.getState(accountTab+"_"+args[0]); 
    stub.getState(accountTab+"_"+args[1]); 

    // get saving on acc1
    int bal1 = Integer.parseInt(stub.getState(savingTab+"_"+args[0])); 
    int bal2 = Integer.parseInt(stub.getState(checkingTab+"_"+args[1])); 
    int totalbal = bal1+bal2; 

    // update balance
    stub.putState(checkingTab+"_"+args[0],0+""); 
    stub.putState(savingTab+"_"+args[1],totalbal+""); 
  }

  // args: acc
  private void getBalance(ChaincodeStub stub, String[] args){
    // get account
    stub.getState(accountTab+"_"+args[0]); 
     // get saving on acc1
    int bal1 = Integer.parseInt(stub.getState(savingTab+"_"+args[0])); 
    int bal2 = Integer.parseInt(stub.getState(checkingTab+"_"+args[0])); 
    int totalbal = bal1+bal2; 
  }

  // args: acc, amount
  private void updateBalance(ChaincodeStub stub, String[] args){
    // get account
    stub.getState(accountTab+"_"+args[0]); 
     // get saving on acc1
    int bal = Integer.parseInt(stub.getState(checkingTab+"_"+args[0]) + Integer.parseInt(args[1])); 
    // update balance
    stub.putState(checkingTab+"_"+args[0], bal+""); 
  }

  // args: acc, amount
  private void updateSaving(ChaincodeStub stub, String[] args){
    // get account
    stub.getState(accountTab+"_"+args[0]); 
     // get saving on acc1
    int bal = Integer.parseInt(stub.getState(savingTab+"_"+args[0]) + Integer.parseInt(args[1])); 
    // update balance
    stub.putState(savingTab+"_"+args[0], bal+""); 
  }

  // args: acc, acc2, amount
  private void sendPayment(ChaincodeStub stub, String[] args){
    // get account
    stub.getState(accountTab+"_"+args[0]); 
    stub.getState(accountTab+"_"+args[2]); 

     // get checking on acc1 and acc2
    int bal1 = Integer.parseInt(stub.getState(checkingTab+"_"+args[0])); 
    int bal2 = Integer.parseInt(stub.getState(checkingTab+"_"+args[1])); 
    int amount = Integer.parseInt(args[2]); 

    // update balance
    stub.putState(checkingTab+"_"+args[0], (bal1-amount)+""); 
    stub.putState(checkingTab+"_"+args[1], (bal2+amount)+""); 
  }

  // args: acc, amount
  private void writeCheck(ChaincodeStub stub, String[] args){
    // get account
    stub.getState(accountTab+"_"+args[0]); 

     // get total balance
    int bal1 = Integer.parseInt(stub.getState(checkingTab+"_"+args[0])); 
    int bal2 = Integer.parseInt(stub.getState(savingTab+"_"+args[0])); 
    int amount = Integer.parseInt(args[1]); 

    // write check, add penality if overdraft
    if (amount < (bal1+bal2)){
      stub.putState(checkingTab+"_"+args[0], (bal1-amount-1)+""); 
    }
    else
      stub.putState(checkingTab+"_"+args[0], (bal1-amount)+""); 
  }


	@Override
	public String query(ChaincodeStub stub, String function, String[] args) {
		return stub.getState(args[0]);
	}

	@Override
	public String getChaincodeID() {
		return "smallbank";
	}

	public static void main(String[] args) throws Exception {
		new SmallbankExample().start(args);
	}

	
}
