// Version key-value store to support querying account at different 
// transactions
package example;

import org.hyperledger.java.shim.ChaincodeBase;
import org.hyperledger.java.shim.ChaincodeStub;
public class VersionKvStore extends ChaincodeBase {
  private static final String VERSION="version";
  private static final String CUR_VER="lastversion"; 
  private static final int MAX_K=100; 
	@Override
	public String run(ChaincodeStub stub, String function, String[] args) {
		switch (function) {
		case "put":
			return put(stub, args);
		case "init":
      return init(stub, args);
    default:
      break; 
		}
		return null;
	}

  // initialize MAX_K values, current version = 0; 
  private String init(ChaincodeStub stub, String[] args){
    for (int i=0; i<MAX_K; i++){
      stub.putState(i+"_0", "0"); 
    }
    stub.putState(CUR_VER, "0"); 
    return null;
  }

  // put
  // args: k,v
  private String put(ChaincodeStub stub, String[] args){
    String txid = stub.getUuid(); 
    // update versions
    stub.putState(args[0]+"_"+txid, args[1]); 
    int k=Integer.parseInt(args[0]); 

    String prev_v = stub.getState(CUR_VER); 
    for (int i=0; i<MAX_K; i++)
      if (i!=k){
        String pre_val = stub.getState(i+"_"+prev_v);
        stub.putState(i+"_"+txid, pre_val); // keep old version 
      }
    stub.putState(CUR_VER, txid); 
    return null; 
  }

  // query value of an account at specific tx
  // k,txid 
	@Override
	public String query(ChaincodeStub stub, String function, String[] args) {
		return stub.getState(args[0]+"_"+args[1]);
	}

	@Override
	public String getChaincodeID() {
		return "VersionKvStore";
	}

	public static void main(String[] args) throws Exception {
		new VersionKvStore().start(args);
	}

	
}
