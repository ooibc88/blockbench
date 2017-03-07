var Web3 = require('web3');
var BigNumber = require('bignumber.js');

var web3 = new Web3();
web3.setProvider(new web3.providers.HttpProvider('http://localhost:8545'));

var ioABI = [{"constant":true,"inputs":[{"name":"key","type":"bytes20"}],"name":"get","outputs":[{"name":"","type":"bytes"}],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"start_key","type":"uint256"},{"name":"size","type":"uint256"},{"name":"signature","type":"uint256"}],"name":"scan","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"start_key","type":"uint256"},{"name":"size","type":"uint256"},{"name":"signature","type":"uint256"}],"name":"revert_scan","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"start_key","type":"uint256"},{"name":"size","type":"uint256"},{"name":"signature","type":"uint256"}],"name":"write","outputs":[],"payable":false,"type":"function"},{"constant":false,"inputs":[{"name":"key","type":"bytes20"},{"name":"value","type":"bytes"}],"name":"set","outputs":[],"payable":false,"type":"function"},{"anonymous":false,"inputs":[{"indexed":false,"name":"size","type":"uint256"},{"indexed":false,"name":"signature","type":"uint256"}],"name":"finishWrite","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"name":"size","type":"uint256"},{"indexed":false,"name":"signature","type":"uint256"}],"name":"finishScan","type":"event"}];

var ioContract = web3.eth.contract(ioABI);

var start_key = parseInt(process.argv[2]);
var size = parseInt(process.argv[3]);
var sig = parseInt(process.argv[4]);
var contract = ioContract.at(process.argv[5]);

var arg_sig = new BigNumber(sig);

var timestamp;

var event = contract.finishWrite();
event.watch(function(error, result) {
  var timenow = new Date().getTime();
  if (!result.args.signature.equals(arg_sig)) {
    return;
  }
  console.log(result.args.size.toString(10) + " key-value written. took "
    + (timenow - timestamp)/1000 + " sec");
  process.exit();
});

function sendTxn(start_key, size, sig) {
  web3.personal.unlockAccount(web3.eth.accounts[0], "");
  contract.write(new BigNumber(start_key), new BigNumber(size), arg_sig, {
    from: web3.eth.accounts[0],
    gas: '10000000000000',
    gasPrice: 0
  });
}
sendTxn(start_key, size, sig);
timestamp = new Date().getTime();

