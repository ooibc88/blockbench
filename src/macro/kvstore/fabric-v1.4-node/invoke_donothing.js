'use strict';

const ccUtil = require("./ccutil.js")

if (process.argv.length < 4) {
    console.log("Invalid parameter...");
    console.log("Should be 'node invoke_donothing.js <ordererAddr> <peerAddr> '");
    process.exit(1);
}

var ordererAddr = "grpc://" + process.argv[2];
var peerAddr = "grpc://" + process.argv[3];
var functionName = "";
var args = [];

var channel;
var client;
var peer;
var ccName = "donothing";

var result = new Object;
var start;
// start = new Date()
Promise.resolve().then(()=>{
    return ccUtil.createChannelAndClient(peerAddr, ordererAddr);
}).then((result)=>{
    channel = result.channel;
    client = result.client;
    peer = client.newPeer(peerAddr);
    // var end = new Date() - start
    // console.info('Create channel time: %dms', end)
    return ccUtil.updateE2E(channel, client, peer, ccName, functionName, args);
}).then((txnID)=>{

    result["status"] = "ok";
    result["txID"] = txnID
    console.log(result)
}).catch((err)=>{
    result["status"] = "err";
    result["msg"] = err.message;
    console.log(result)
});
