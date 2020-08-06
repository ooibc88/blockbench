'use strict';

const ccUtil = require("./ccutil.js")

if (process.argv.length < 6) {
    console.log("Invalid parameter...");
    console.log("Should be 'node populate.js <ordererAddr> <peerAddr> <#-of-accounts> <#-of-txns>'");
    process.exit(1);
}

var ordererAddr = "grpc://" + process.argv[2];
var peerAddr = "grpc://" + process.argv[3];
var accountNum = Number(process.argv[4]);
var txnNum = Number(process.argv[5])

var channel;
var client;
var peer;

function getRandomInt(max) {
    return Math.floor(Math.random() * Math.floor(max));
}

var result = new Object;
Promise.resolve().then(()=>{
    return ccUtil.createChannelAndClient(peerAddr, ordererAddr);
}).then((result)=>{
    channel = result.channel;
    client = result.client;
    peer = client.newPeer(peerAddr);

    var txnSeq = [];
    for (let i = 0; i < txnNum; i++) {
        txnSeq.push(i);
    }
    // Invoke txns sequentially. 
    return txnSeq.reduce((prevPromise, txnID)=>{
        return prevPromise.then((prevTxnID)=>{
            // console.log(`Prev Txn ID: ${prevTxnID}\n\n`, );
            var fromAccount = "Acc" + getRandomInt(accountNum);
            var toAccount = "Acc" + getRandomInt(accountNum);
            var amount = "" + getRandomInt(100);
            console.log(`Txn ${txnID} transfers ${amount} from ${fromAccount} to ${toAccount}. `);
            return ccUtil.updateE2E(channel, client, peer, "analytic", "sendPayment", [fromAccount, toAccount, amount])
        });
    }, Promise.resolve());
}).then((txnID)=>{
    // console.log("Final txn ID: ", txnID);
}).catch((err)=>{
    result["status"] = "err";
    result["msg"] = err.message;
    console.log(result)
});
