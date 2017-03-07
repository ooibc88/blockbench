// IO-heavy, for testing execution engine
// Put and get many keys
package example;

import org.hyperledger.java.shim.ChaincodeBase;
import org.hyperledger.java.shim.ChaincodeStub;
public class IOHeavy extends ChaincodeBase {
  private static String SIZE="size"; 
  private static String TIME="time"; 
	@Override
	public String run(ChaincodeStub stub, String function, String[] args) {
    switch (function){
      case "init":
        init(stub, function, args); 
        break; 
      case "test":
        test(stub, function, args); 
        break; 
      default:
        break; 
    }
		return null;
	}

  // args: max size
  private void init(ChaincodeStub stub, String function, String[] args){
    stub.putState(SIZE, args[0]); 
  }

  private void test(ChaincodeStub stub, String function, String[] args){
    long startTime = System.currentTimeMillis(); 
    int size = Integer.parseInt(stub.getState(SIZE)); 
    // write
    for (int i=0; i<size; i++)
      stub.putState(i+"", i+""); 

    // read
    for (int i=size-1; i>=0; i--)
      stub.getState(i+""); 

    long endTime = System.currentTimeMillis(); 
    stub.putState(TIME, (endTime-startTime)+""); 
  }

	@Override
	public String query(ChaincodeStub stub, String function, String[] args) {
    return stub.getState(TIME);
	}

	@Override
	public String getChaincodeID() {
		return "IO Heavy";
	}

	public static void main(String[] args) throws Exception {
		new IOHeavy().start(args);
	}
}
