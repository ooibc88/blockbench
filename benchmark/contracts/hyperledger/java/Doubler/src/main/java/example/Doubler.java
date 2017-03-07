// Implement Doubler smart contract on Ethereum
// 0xfD..B4D
import org.hyperledger.java.shim.ChaincodeBase;
import org.hyperledger.java.shim.ChaincodeStub;
public class Doubler extends ChaincodeBase {
  private static final String OWNER="owner_doubler"; 
  private static final String FEES="onwer_fee"; 
  private static final String BALANCE="balance"; 
  private static final String PLAYER="participant"; 
  private static final String DEPOSIT="deposit"; 
  private static final String LIST_LENGTH="list_length"; 
  private static final String PAYOUT_IDX="payout"; 
	@Override
	public String run(ChaincodeStub stub, String function, String[] args) {
		switch (function) {
    case "init":
      init(stub, args); 
      break;
		case "enter":
			return enter(stub, args);
		case "collectFees":
      return collectFees(stub, args);
    case "setOwner":
      return setOwner(stub, args); 
    default:
      break; 
		}
		return null;
	}

  // set owner
  // args: owner
  private String init(ChaincodeStub stub, String[] args){
    stub.putState(OWNER, args[0]); 
    stub.putState(FEES, 0+""); 
    stub.putState(BALANCE, 0+""); 
    stub.putState(LIST_LENGTH, 0+""); 
    stub.putState(PAYOUT_IDX, 0+""); 
    return null; 
  }

  // enter 
  // args: sender, value
  private String enter(ChaincodeStub stub, String[] args){
    // add sender to list
    int len = Integer.parseInt(stub.getState(LIST_LENGTH)); 
    stub.putState(LIST_LENGTH, (len+1)+""); 
    stub.putState(PLAYER+"_"+len, args[0]); 
    stub.putState(DEPOSIT+"_"+len, args[1]); 

    // collect fee
    int fee = Integer.parseInt(stub.getState(FEES)); 
    fee+= Integer.parseInt(args[1])/10; 
    stub.putState(FEES, fee+""); 

    // update balance
    int bal = Integer.parseInt(stub.getState(BALANCE)); 
    bal += Integer.parseInt(args[1]); 
    stub.putState(BALANCE, bal+""); 

    int payoutIdx = Integer.parseInt(stub.getState(PAYOUT_IDX)); 
    if (payoutIdx <len){
      int payoutLim = Integer.parseInt(stub.getState(DEPOSIT+"_"+payoutIdx)); 
      if (bal > payoutLim*2){
        int amount = 2*(payoutLim - payoutLim/10); 
        // pay to the earlier player 
        credit(stub, stub.getState(PLAYER+"_"+payoutIdx), amount); 
        // move to the next player
        stub.putState(PAYOUT_IDX, (payoutIdx+1)+""); 
        // update balance
        bal -= 2*payoutLim; 
        stub.putState(BALANCE, bal+""); 
      }
    }
    return null; 
  }

  private void credit(ChaincodeStub stub, String acc, int amount){
    String state = stub.getState(BALANCE+"_"+acc); 
    int bal = 0; 
    if (state!=null)
      bal = Integer.parseInt(state); 
    bal +=amount; 
    stub.putState(BALANCE+"_"+acc, bal+""); 
  }

  // send fees back to owner
  // args: sender
  private String collectFees(ChaincodeStub stub, String[] args){
    if (!stub.getState(OWNER).equals(args[0]))
      return "{\"Error\": \"Failed at collecting fees\"}"; 
    
    int fee = Integer.parseInt(stub.getState(FEES)); 
    if (fee>0){
      credit(stub, args[0], fee); 
      // reset fee
      stub.putState(FEES, 0+""); 
    }
    return null; 
  }

  // send fees back to owner
  // args: sender, new_owner
  private String setOwner(ChaincodeStub stub, String[] args){
    if (!stub.getState(OWNER).equals(args[0]))
      return "{\"Error\": \"Failed at setting owner\"}"; 
    stub.putState(OWNER, args[1]);  
    return null; 
  }

	@Override
	public String query(ChaincodeStub stub, String function, String[] args) {
		return stub.getState(args[0]);
	}

	@Override
	public String getChaincodeID() {
		return "Doubler";
	}

	public static void main(String[] args) throws Exception {
		new Doubler().start(args);
	}

	
}
