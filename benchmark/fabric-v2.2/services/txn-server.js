/*
 * Copyright IBM Corp. All Rights Reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

'use strict';

const { Gateway, Wallets, DefaultEventHandlerStrategies  } = require('fabric-network');
const fs = require('fs');
const path = require('path');
const express = require('express');
const bodyParser = require('body-parser');


const argsLen = process.argv.length;

function helper() {
    console.error("Expected usage: ")
    console.error("\tnode txn-server.js <channelName> <contractName> [open_loop|closed_loop] <port>")
    console.error("\tThe 'open_loop' indicates the invocation will return once the transaction is submitted to orderers. The 'closed_loop' will block until peers finish validation. Their configuration may impact on invocation transactions, not on queries. ")

}
if ( argsLen <= 5) {
    console.error(`Too few arguments, expect 6`);
    helper();
    process.exit(1);
}
const channelName = process.argv[2];
const contractName = process.argv[3];

if (!(process.argv[4] == "open_loop" || process.argv[4] == "closed_loop")) {
    console.error(`Invalid invocation mode ${process.argv[4]}. `);
    helper();
    process.exit(1);
}
const isOpenLoopMode = process.argv[4] == "open_loop";

const port = Number(process.argv[5]);

async function getChannel(channelName, contractName) {
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
            process.exit(1);
        }

        // Create a new gateway for connecting to our peer node.
        var mode;
        if (isOpenLoopMode) {
            mode = DefaultEventHandlerStrategies.NONE;
        } else {
            mode = DefaultEventHandlerStrategies.MSPID_SCOPE_ALLFORTX
        }
        const gateway = new Gateway();
        await gateway.connect(ccp, 
            { 
            wallet, identity: 'appUser', 
            discovery: { enabled: true, asLocalhost: true}, 
            eventHandlerOptions: {
                strategy: mode
            }  
        });

        // Get the network (channel) our contract is deployed to.
        const network = await gateway.getNetwork(channelName);

        // Get the contract from the network.
        const contract = network.getContract(contractName);
        console.log(`Contract ${contractName} on Channel ${channelName} has been setup... }`);
        return contract;

    } catch (error) {
        console.error(`Failed to set up the contract and channel: ${error}`);
        process.exit(1);
    }
}

getChannel(channelName, contractName).then((contract)=>{
    const app = express();

    app.listen(port, () => {
        console.log(`Server running on port ${port}. Is the open-loop mode? ${isOpenLoopMode}`);
    })

    app.use(bodyParser.json());

    app.post("/invoke", (req, res) => { 
        var txn;
        console.log("Receive request: ", req.body)
        const funcName = req.body["function"];
        const args = req.body["args"];
        console.log(`Receive funcName: ${funcName}, args: ${args}`);
        var start; 
        new Promise((resolve, reject)=>{
            txn = contract.createTransaction(funcName);
            start = new Date();
            resolve(txn.submit(...args));
        }).then(()=>{
            var end = new Date() - start
            const txnID = txn.getTransactionId();
            res.json({"status": "0", "txnID": txnID, "latency_ms": end});
        }).catch((error)=>{
            console.error(`Failed to invoke with error: ${error}`);
            res.json({"status": "1", "message": error.message});
        });
    });

    app.get("/query", (req, res) => { 
        // console.log("Receive request: ", req.body)
        const funcName = req.query.function;
        const args = req.query.args.split(',');
        console.log(`Receive funcName: ${funcName}, args: ${args}`);
        var start; 
        new Promise((resolve, reject)=>{
            start = new Date();
            resolve(contract.evaluateTransaction(funcName, ...args));
        }).then((result)=>{
            var end = new Date() - start
            res.json({"status": "0", "result": result.toString(), "latency_ms": end});
        }).catch((error)=>{
            console.error(`Failed to query with error: ${error}`);
            res.json({"status": "1", "message": error.message});
        });
    });
})
