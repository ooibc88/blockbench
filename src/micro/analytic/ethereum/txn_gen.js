const TXNS_PER_BLOCK = 3
const INVOKE_TIMES = 1000

var Web3 = require('web3');
var web3 = new Web3();
web3.setProvider(new web3.providers.HttpProvider());

var accounts = web3.personal.listAccounts;
var len = accounts.length;

var t = 0;

function gen_txns() {
  for (var i = 0; i < TXNS_PER_BLOCK; ++i) {
    var x = Math.floor(Math.random() * len); 
    var y = Math.floor(Math.random() * len); 
    var val = Math.floor(Math.random() * 100); 
    try {
      web3.personal.unlockAccount(accounts[x], "");
      web3.personal.sendTransaction({
        from: accounts[x],
        to: accounts[y],
        value: val,
        gasPrice: 0,
        gas: 100000
      });
    } catch (error) {
      console.log("account: " + accounts[x]);
      console.log(error);
    }
    ++t;
    if (t == INVOKE_TIMES) {
        process.exit();
    }
  }
}

web3.eth.filter('latest').watch(gen_txns);
