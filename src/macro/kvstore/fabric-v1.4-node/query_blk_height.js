'use strict';

const ccUtil = require("./ccutil.js")

if (process.argv.length < 2) {
    console.log("Invalid parameter...");
    console.log("Should be 'node query_blk_height.js <ordererAddr> <peerAddr> '");
    process.exit(1);
}

var ordererAddr = "grpc://" + process.argv[2];
var peerAddr = "grpc://" + process.argv[3];

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
    return channel.queryInfo();
}).then((info)=>{
    result["status"] = "ok";
    result["height"] = "" + info.height.low;
    console.log(result)
}).catch((err)=>{
    result["status"] = "err";
    result["msg"] = err.message;
    console.log(result)
});
