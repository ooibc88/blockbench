'use strict';

const ccUtil = require("./ccutil.js")

if (process.argv.length < 7) {
    console.log("Invalid parameter...");
    console.log("Should be 'node bench_q2.js <ordererAddr> <peerAddr> <start block> <end block> <account>'");
    process.exit(1);
}

var ordererAddr = "grpc://" + process.argv[2];
var peerAddr = "grpc://" + process.argv[3];
var startBlk =  parseInt(process.argv[4]);
var endBlk =  parseInt(process.argv[5]);
var account =  process.argv[6];

var channel;
var start;
var maxValue = 0;

function maxTxnValueInBlkOnAccount(block, account){
    let tx_filters = block.metadata.metadata[2]
    var maxBlkValue = 0;
    for (var index = 0; index < block.data.data.length; index++) {
        var channel_header = block.data.data[index].payload.header.channel_header;
        if (tx_filters[index] === 0) {
            var txActions = block.data.data[index].payload.data.actions;
            for (var i = 0; i < txActions.length; i++) {
                var writes = txActions[i].payload.action.proposal_response_payload.extension.results.ns_rwset[0].rwset.writes;
                var txnValue;
                var involved = false;
                for (var j = 0; j < writes.length;j++) {
                    // console.log("Write[j]: ", writes[j]);
                    if (writes[j].key == "Value") {
                        txnValue = parseInt(writes[j].value);
                    }
                    if (writes[j].key == account) {
                        involved = true;
                    }
                }

                if (involved && txnValue > maxBlkValue) {
                    maxBlkValue = txnValue;
                }
            }
        }
    }
    return txnValue;
}

Promise.resolve().then(()=>{
    return ccUtil.createChannelAndClient(peerAddr, ordererAddr);
}).then((result)=>{
    channel = result.channel;
    var blkNums = [];
    for (var i = startBlk; i <= endBlk; i++) {
        blkNums.push(i);
    }
    start = new Date();
    return blkNums.reduce((prevBlkPromise, blkNum)=>{
        return prevBlkPromise.then(()=>{
            return channel.queryBlock(blkNum).then((block)=>{
                var maxBlkValue = maxTxnValueInBlkOnAccount(block, account);
                if (maxValue < maxBlkValue) {
                    maxValue = maxBlkValue;
                }
            });
        });
    }, Promise.resolve());
}).then(()=>{
    var end = new Date() - start;
    console.log(`Max Transaction Value = ${maxValue} for account ${account} from block ${startBlk} to ${endBlk} in ${end} ms. `)
}).catch((err)=>{
    console.log("Error: ", err.stack);
});
