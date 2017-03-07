// CPU-heavy, for testing execution engine
// Sort many random keys 
package example;

import org.hyperledger.java.shim.ChaincodeBase;
import org.hyperledger.java.shim.ChaincodeStub;
import java.util.*; 
public class CPUHeavy extends ChaincodeBase {
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
    int size = Integer.parseInt(stub.getState(SIZE)); 
    ArrayList<Integer> lists = new ArrayList(); 
    Random rand = new Random(); 
    for (int i=0; i<size; i++)
      lists.add(rand.nextInt()); 

    long startTime = System.currentTimeMillis(); 
    Collections.sort(lists);  
    long endTime = System.currentTimeMillis(); 
    stub.putState(TIME, (endTime-startTime)+""); 
  }

	@Override
	public String query(ChaincodeStub stub, String function, String[] args) {
    return stub.getState(TIME);
	}

	@Override
	public String getChaincodeID() {
		return "CPU Heavy";
	}

	public static void main(String[] args) throws Exception {
		new CPUHeavy().start(args);
	}
}
