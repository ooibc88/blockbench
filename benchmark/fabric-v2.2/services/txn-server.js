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
if ( argsLen <= 4) {
    console.error(`Too few arguments, expect 5`);
    console.error("Expected usage: ")
    console.error("\tnode txn-server.js <channelName> <contractName> <port>")
    process.exit(1);
}
const channelName = process.argv[2];
const contractName = process.argv[3];
const port = Number(process.argv[4]);

async function main(channelName, contractName) {
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
        const gateway = new Gateway();
        await gateway.connect(ccp, 
            { 
            wallet, identity: 'appUser', 
            discovery: { enabled: true, asLocalhost: true}, 
            eventHandlerOptions: {
                // submitTx() will return as soon as the txn is sent to orderer
                strategy: DefaultEventHandlerStrategies.NONE 
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

main(channelName, contractName).then((contract)=>{
    const app = express();

    app.listen(port, () => {
        console.log(`Server running on port ${port}`);
    })

    app.use(bodyParser.json());

    app.post("/invoke", (req, res) => { 
        var txn;
        // console.log("Receive request: ", req.body)
        const funcName = req.body["function"];
        const args = req.body["args"];
        // console.log(`Receive funcName: ${funcName}, args: ${args}`);

        new Promise((resolve, reject)=>{
            txn = contract.createTransaction(funcName);
            resolve(txn.submit(...args));
        }).then(()=>{
            const txnID = txn.getTransactionId();
            res.json({"status": 0, "txnID": txnID});
        }).catch((error)=>{
            console.error(`Failed to invoke with error: ${error}`);
            res.json({"status": 1, "message": error.message});
        });

    });
})
