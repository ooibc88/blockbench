var Web3 = require('web3');
var BigNumber = require('bignumber.js');

var web3 = new Web3();
web3.setProvider(new web3.providers.HttpProvider('http://localhost:8545'));

var sorterABI = [{"constant":false,"inputs":[{"name":"size","type":"uint256"},{"name":"signature","type":"uint256"}],"name":"sort","outputs":[],"payable":false,"type":"function"},{"anonymous":false,"inputs":[{"indexed":false,"name":"size","type":"uint256"},{"indexed":false,"name":"signature","type":"uint256"}],"name":"finish","type":"event"}];

var sorterContract = web3.eth.contract(sorterABI);

web3.personal.unlockAccount(web3.eth.accounts[0], "");

var sorter = sorterContract.new(
  {
    from: web3.eth.accounts[0], 
    data: '0x6060604052341561000c57fe5b5b6102c28061001c6000396000f30060606040526000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff1680637b395ec21461003b575bfe5b341561004357fe5b6100626004808035906020019091908035906020019091905050610064565b005b61006c610282565b60008360405180591061007c5750595b908082528060200260200182016040525b509150600090505b81518110156100cb5780840382828151811015156100af57fe5b90602001906020020181815250505b8080600101915050610095565b6100db8260006001855103610121565b7fd596fdad182d29130ce218f4c1590c4b5ede105bee36690727baa6592bd2bfc88484604051808381526020018281526020019250505060405180910390a15b50505050565b600060006000818314156101345761027a565b84925083915085600286860381151561014957fe5b04860181518110151561015857fe5b9060200190602002015190505b8183111515610251575b80868481518110151561017e57fe5b90602001906020020151101561019b57828060010193505061016f565b5b85828151811015156101aa57fe5b906020019060200201518110156101c95781806001900392505061019c565b818311151561024c5785828151811015156101e057fe5b9060200190602002015186848151811015156101f857fe5b90602001906020020151878581518110151561021057fe5b906020019060200201888581518110151561022757fe5b9060200190602002018281525082815250505082806001019350508180600190039250505b610165565b8185101561026557610264868684610121565b5b8383101561027957610278868486610121565b5b5b505050505050565b6020604051908101604052806000815250905600a165627a7a723058201f3ad7bd41cd34c49cebb9ecf9ca80e0a7c4a68c538f52a05b1a6fe2616ebf370029',
    gas: '4700000'
  },
  function (e, contract) {
    if (typeof contract.address !== 'undefined') {
      console.log("contract mined: " + contract.address);
      
      var timestamp;
      var event = contract.finish();
      event.watch(function(error, result) {
        var timenow = new Date().getTime();
        console.log(result.args.size.toString(10) + " numbers sorted. took "
          + (timenow - timestamp)/1000 + " sec");
	process.exit();
      });

      function sendTxn(size, signature) {
        web3.personal.unlockAccount(web3.eth.accounts[0], "");
        console.log(contract.sort(new BigNumber(size), new BigNumber(signature),
          {
            from: web3.eth.accounts[0],
            gas: '10000000000000',
            gasPrice: 0
          }
        ));
      }

      console.log("before send: " + new Date().getTime());
      sendTxn(parseInt(process.argv[2]), 1);
      timestamp = new Date().getTime();
      console.log("after send: " + timestamp);
    }
  }
)
