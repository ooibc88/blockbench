'use strict';

const ccUtil = require("./ccutil.js")

if (process.argv.length < 6) {
    console.log("Invalid parameter...");
    console.log("Should be 'node bench_q1.js <ordererAddr> <peerAddr> <start block> <end block>'");
    process.exit(1);
}

var ordererAddr = "grpc://" + process.argv[2];
var peerAddr = "grpc://" + process.argv[3];
var startBlk =  parseInt(process.argv[4]);
var endBlk =  parseInt(process.argv[5]);

var channel;
var start;
var totalValue = 0;

function computeTxnValueInBlk(block){
    let tx_filters = block.metadata.metadata[2]
    var blkValue = 0;
    for (var index = 0; index < block.data.data.length; index++) {
        var channel_header = block.data.data[index].payload.header.channel_header;
        if (tx_filters[index] === 0) {
            var txActions = block.data.data[index].payload.data.actions;
            for (var i = 0; i < txActions.length; i++) {
                var writes = txActions[i].payload.action.proposal_response_payload.extension.results.ns_rwset[0].rwset.writes;
                for (var j = 0; j < writes.length;j++) {
                    // console.log("Write[j]: ", writes[j]);
                    if (writes[j].key == "Value") {
                        blkValue += parseInt(writes[j].value);
                    }
                }
            }
        }
    }
    return blkValue;
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
                totalValue += computeTxnValueInBlk(block);
            });
        });
    }, Promise.resolve());
}).then(()=>{
    var end = new Date() - start;
    console.log(`Total Transaction Value = ${totalValue} from block ${startBlk} to ${endBlk} in ${end} ms. `)
}).catch((err)=>{
    console.log("Error: ", err.stack);
});
