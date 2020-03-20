'use strict';

const ccUtil = require("./ccutil.js")

if (process.argv.length < 5) {
    console.log("Invalid parameter...");
    console.log("Should be 'node invoke.js <ordererAddr> <peerAddr> <functionName> [args...]'");
    process.exit(1);
}

var ordererAddr = "grpc://" + process.argv[2];
var peerAddr = "grpc://" + process.argv[3];
var functionName = process.argv[4]
var args = process.argv.slice(5)

var channel;
var client;
var peer;

var result = new Object;
Promise.resolve().then(()=>{
    return ccUtil.createChannelAndClient(peerAddr, ordererAddr);
}).then((result)=>{
    channel = result.channel;
    client = result.client;
    peer = client.newPeer(peerAddr);
    return ccUtil.updateE2E(channel, client, peer, functionName, args);
}).then((txnID)=>{
    result["status"] = "ok";
    result["txID"] = txnID
    console.log(result)
}).catch((err)=>{
    result["status"] = "err";
    result["msg"] = err.message;
    console.log(result)
});
