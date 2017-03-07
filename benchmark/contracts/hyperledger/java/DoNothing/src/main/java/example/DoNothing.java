// Do nothing, to test Consensus layer 

package example;

import org.hyperledger.java.shim.ChaincodeBase;
import org.hyperledger.java.shim.ChaincodeStub;
public class DoNothing extends ChaincodeBase {
	@Override
	public String run(ChaincodeStub stub, String function, String[] args) {
    stub.getState("0"); 
		return null;
	}

	@Override
	public String query(ChaincodeStub stub, String function, String[] args) {
    return "";
	}

	@Override
	public String getChaincodeID() {
		return "Do nothing";
	}

	public static void main(String[] args) throws Exception {
		new DoNothing().start(args);
	}
}
