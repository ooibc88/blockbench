// Implement WavesPresale smart contract on Ethereum
// 0xae...9331

package example;

import org.hyperledger.java.shim.ChaincodeBase;
import org.hyperledger.java.shim.ChaincodeStub;
public class WavesPresale extends ChaincodeBase {
  private static final String OWNER="owner_waves_presale"; 

	@Override
	public String run(ChaincodeStub stub, String function, String[] args) {
		switch (function) {
    case "init":
      init(stub, args); 
      break;
		case "changeOwner":
			return changeOwner(stub, args);
		case "newSale":
      return newSale(stub, args);
    case "getNumOfSalesWithSameId":
      return getNumOfSalesWithSameId(stub, args); 
    case "getSaleDate":
      return getSaleDate(stub, args); 
    default:
      break; 
		}
		return null;
	}

  // set owner
  // args: owner
  private String init(ChaincodeStub stub, String[] args){
    stub.putState(OWNER, args[0]); 
    return null; 
  }

  // change owner
  // args: sender, newowner
  private String changeOwner(ChaincodeStub stub, String[] args){
    if (!stub.getState(OWNER).equals(args[0]))
      return "{\"Error\": \"Failed at changing owner\"}"; 
    stub.putState(OWNER, args[1]); 
    return null; 
  }

  // new sale
  // args: sender, txid, amount, timestamp
  private String newSale(ChaincodeStub stub, String[] args){
    if (!stub.getState(OWNER).equals(args[0]))
      return "{\"Error\": \"Failed at issuing new sale owner\"}"; 
    String curState = stub.getState(args[1]); 
    if (curState!=null)
      curState +="\t"+args[2]+":"+args[3]; 
    else 
      curState = "\t"+args[2]+":"+args[3]; 
    stub.putState(args[1], curState); 
    return null; 
  }

  // get number of sales for an ID
  // args: txid
  private String getNumOfSalesWithSameId(ChaincodeStub stub, String[] args){
    String curState = stub.getState(args[0]); 
    return curState.split("\t").length+""; 
  }

  // get sale date for an ID
  // args: txid, idx
  private String getSaleDate(ChaincodeStub stub, String[] args){
    String curState = stub.getState(args[0]); 
    int idx = Integer.parseInt(args[1]); 
    if (curState!=null){
      String[] ss = curState.split("\t"); 
      if (idx<ss.length)
        return ss[idx];
      else
        return "{\"Error\": \"Failed at getting SaleDate owner\"}"; 
    }
    else return "{\"Error\": \"Failed at getting SaleDate owner\"}";  
  }

	@Override
	public String query(ChaincodeStub stub, String function, String[] args) {
		return stub.getState(args[0]);
	}

	@Override
	public String getChaincodeID() {
		return "WavesPresale";
	}

	public static void main(String[] args) throws Exception {
		new WavesPresale().start(args);
	}

	
}
