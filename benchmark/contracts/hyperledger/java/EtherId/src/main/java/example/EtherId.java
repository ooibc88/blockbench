// Implement WavesPresale smart contract on Ethereum
// 0xae...9331
package example;

import org.hyperledger.java.shim.ChaincodeBase;
import org.hyperledger.java.shim.ChaincodeStub;

// encoding of domain: owner \t price \t value
// money account: account_name => value
public class EtherId extends ChaincodeBase {

  private static final String OWNER="owner_etherid"; 
  private static final String BALANCE="balance"; 
	@Override
	public String run(ChaincodeStub stub, String function, String[] args) {
		switch (function) {
    case "init":
      init(stub, args); 
      break;
		case "getId":
			return getId(stub, args);
		case "getDomain":
      return getDomain(stub, args);
    case "changeDomain":
      return changeDomain(stub, args); 
    case "changeId":
      return changeId(stub, args); 
    default:
      break; 
		}
		return null;
	}

  private String getOwner(String state){
    return state.split("\t")[0]; 
  }
  private String getPrice(String state){
    return state.split("\t")[1]; 
  }
  private String getValue(String state){
    return state.split("\t")[2]; 
  }
  private String createDomain(String owner, String price, String value){
    return owner+"\t"+price+"\t"+value; 
  }

  // set owner
  // args: owner
  private String init(ChaincodeStub stub, String[] args){
    stub.putState(OWNER, args[0]); 
    return null; 
  }

  // get value for a domain
  // args: domain
  private String getId(ChaincodeStub stub, String[] args){
    String state = stub.getState(args[0]); 
    if (state==null)
      return "{\"Error\": \"Failed at getId\"}"; 
    else return getValue(state); 
  }

  // get a domain
  // args: domain
  private String getDomain(ChaincodeStub stub, String[] args){
    String state = stub.getState(args[0]); 
    if (state==null)
      return "{\"Error\": \"Failed at getId\"}"; 
    else return state; 
  }

  // change domain
  // args: sender, buy_value, domain, newprice (to be set), newowner
  private String changeDomain(ChaincodeStub stub, String[] args){
    String state = stub.getState(args[2]); 
    if (state==null){ // domain has not been registered
      stub.putState(args[2], createDomain(args[0], args[3], ""));
      debit(stub, args[0], args[1]); 
    }
    else{
      // check if the sender is the owner
      if (!getOwner(state).equals(args[0]) || Integer.parseInt(getPrice(state)) > Integer.parseInt(args[1]))
        return "{\"Error\": \"Failed at changeDomain\"}"; 
      // transfer to new owner
      stub.putState(args[2], createDomain(args[4], args[3], "")); 
      // credit money to the old owner
      credit(stub, args[0], getPrice(state)); 
      debit(stub, args[4], getPrice(state)); 
    }
    return null;
  }

  // change value of existing domain
  // args: sender, domain, new value
  private String changeId(ChaincodeStub stub, String[] args){
    String state = stub.getState(args[1]); 
    if (state==null || (state!=null && !getOwner(state).equals(args[0])))
      return "{\"Error\": \"Failed at changeId\"}"; 
    stub.putState(args[1], createDomain(args[0], getPrice(state), args[2])); 
    return null;
  }

  private void debit(ChaincodeStub stub, String acc, String amount){
    String bal = stub.getState(BALANCE+"_"+acc); 
    int accval=0; 
    if (bal!=null)
      accval = Integer.parseInt(bal); 
    accval -= Integer.parseInt(amount); 
    stub.putState(BALANCE+"_"+acc, accval+""); 
  }
  private void credit(ChaincodeStub stub, String acc, String amount){
    String bal = stub.getState(BALANCE+"_"+acc); 
    int accval=0; 
    if (bal!=null)
      accval = Integer.parseInt(bal); 
    accval += Integer.parseInt(amount); 
    stub.putState(BALANCE+"_"+acc, accval+""); 
  }


	@Override
	public String query(ChaincodeStub stub, String function, String[] args) {
		return stub.getState(args[0]);
	}

	@Override
	public String getChaincodeID() {
		return "EtherId";
	}

	public static void main(String[] args) throws Exception {
		new EtherId().start(args);
	}

	
}
