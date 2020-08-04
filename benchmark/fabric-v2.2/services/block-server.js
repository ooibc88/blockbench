/*
 * Copyright IBM Corp. All Rights Reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

'use strict';

const { Gateway, Wallets, GatewayOptions, DefaultEventHandlerStrategies  } = require('fabric-network');
const fs = require('fs');
const path = require('path');
const express = require('express');
const bodyParser = require('body-parser');


const argsLen = process.argv.length;
if ( argsLen <= 3) {
    console.error(`Too few arguments, expect 4`);
    console.error("Expected usage: ")
    console.error("\tnode block-server.js <channelName> <port>")
    process.exit(1);
}
const channelName = process.argv[2];
const port = Number(process.argv[3]);
var blkTxns = {};
var height = 0;

async function getChannel(channelName) {
    try {
        // load the network configuration
        const ccpPath = path.resolve(__dirname, '..', 'organizations', 'peerOrganizations', 'org1.example.com', 'connection-org1.json');
        let ccp = JSON.parse(fs.readFileSync(ccpPath, 'utf8'));

        // Create a new file system based wallet for managing identities.
        const walletPath = path.join(process.cwd(), 'wallet');
        const wallet = await Wallets.newFileSystemWallet(walletPath);
        // console.log(`Wallet path: ${walletPath}`);

        // Check to see if we've already enrolled the user.
        const identity = await wallet.get('appUser');
        if (!identity) {
            console.log('An identity for the user "appUser" does not exist in the wallet');
            console.log('Run the registerUser.js application before retrying');
            return;
        }

        // Create a new gateway for connecting to our peer node.
        const gateway = new Gateway();
        await gateway.connect(ccp, 
            { 
            wallet, identity: 'appUser', 
            discovery: { enabled: true, asLocalhost: true}, 
        });

        // Get the network (channel) our contract is deployed to.
        const network = await gateway.getNetwork(channelName);
        console.log(`Channel ${channelName} has been setup... }`);
        return network;

    } catch (error) {
        console.error(`Failed to set up the contract and channel: ${error}`);
        process.exit(1);
    }
}

getChannel(channelName).then((network)=>{
    const listener = async (event) => {
        try {
            height = Number(event.blockNumber) + 1;
            const blkNum = "" + event.blockNumber; //conver to str
            const block = event.blockData;
            blkTxns[blkNum] = [];
            let tx_filters = block.metadata.metadata[2]
            for (var index = 0; index < block.data.data.length; index++) {
                var channel_header = block.data.data[index].payload.header.channel_header;
                if (tx_filters[index] === 0) {
                    blkTxns[blkNum].push(channel_header.tx_id)
                }
            }
            console.log(`Block ${blkNum} has txns ${blkTxns[blkNum]}. `);

        } catch (error) {
            console.error(`Failed to listen for blocks: ${error}`);
        }
    };
    return network.addBlockListener(listener, {startBlock: 1});
}).then(()=>{
    const app = express();

    app.listen(port, () => {
        console.log(`Server running on port ${port}`);
    })

    app.use(bodyParser.json());

    app.get("/block", (req, res) => { 
        const blkNum = "" + req.body["blk_num"]; //convert to string
        const txns = blkTxns[blkNum];
        if (txns === undefined) {
            res.json({"status": 1, "message": "Block " + blkNum + " does not exist. "});
        } else {
            res.json({"status": 0, "txns": txns});
        }
    });

    app.get("/height", (req, res) => { 
        res.json({"status": 0, "height": height});
    });
}).catch((error)=>{
    console.error(`Failed to set up the contract and channel: ${error}`);
    process.exit(1);

})
