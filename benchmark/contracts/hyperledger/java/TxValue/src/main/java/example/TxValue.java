// Store values of Tx, for the query that sum values of tx from block i to block j

package example;

import org.hyperledger.java.shim.ChaincodeBase;
import org.hyperledger.java.shim.ChaincodeStub;
public class TxValue extends ChaincodeBase {
	@Override
	public String run(ChaincodeStub stub, String function, String[] args) {
		switch (function) {
		case "put":
			return put(stub, args);
	  default:
      break; 
		}
		return null;
	}

  // put
  // args: k,v
  private String put(ChaincodeStub stub, String[] args){
    String txid = stub.getUuid(); 
    stub.putState(txid, args[1]); 
    return null; 
  }

  // query value of an account at specific tx
  // txid 
	@Override
	public String query(ChaincodeStub stub, String function, String[] args) {
		return stub.getState(args[0]);
	}

	@Override
	public String getChaincodeID() {
		return "TxValue";
	}

	public static void main(String[] args) throws Exception {
		new TxValue().start(args);
	}

	
}
