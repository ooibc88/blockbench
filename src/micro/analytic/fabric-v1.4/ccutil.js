// 'use strict';


const path = require('path');
const fs = require('fs');
const os = require('os')

const Client = require('fabric-client');

const client = new Client();
const keyPath = path.join(__dirname, "../../../../benchmark/fabric-v1.4/four-nodes-docker/crypto_config/peerOrganizations/org1.example.com/users/User1@org1.example.com/msp/keystore/2f237b2473e0e9f1d63d2715b1c0acdf376a481cbf0259befc1355361229209a_sk")
const certPath = path.join(__dirname, "../../../../benchmark/fabric-v1.4/four-nodes-docker/crypto_config/peerOrganizations/org1.example.com/users/User1@org1.example.com/msp/signcerts/User1@org1.example.com-cert.pem")
const MSPID = "Org1MSP"
const channelName = "rpcchannel"
var txIdObject;

module.exports.createChannelAndClient = function (peerAddr, ordererAddr) {
    return Promise.resolve().then(()=>{
        const cryptoSuite = Client.newCryptoSuite();
        const tmpPath = path.join(os.tmpdir(), 'hfc')
        cryptoSuite.setCryptoKeyStore(Client.newCryptoKeyStore({path: tmpPath}));
        client.setCryptoSuite(cryptoSuite);
        return Client.newDefaultKeyValueStore({path: tmpPath});
    }).then((store)=>{
        if (store) {client.setStateStore(store); }
        const keyPEM = fs.readFileSync(keyPath);
        const certPEM = fs.readFileSync(certPath);
        const createUserOpt = { 
            username: "Org1Peer", 
            mspid: MSPID, 
            cryptoContent: { 
                privateKeyPEM: keyPEM.toString(), 
                signedCertPEM: certPEM.toString() 
            } 
        };
        return client.createUser(createUserOpt);
    }).then((user)=>{
        let channel = client.newChannel(channelName);
        let peer = client.newPeer(peerAddr);
        channel.addPeer(peer);
        var orderer = client.newOrderer(ordererAddr); 
        
        channel.addOrderer(orderer);
        channel.initialize();
        return {channel: channel, client: client};
    }).catch((err)=>{
        console.log("Err: ", err);
    });
}

module.exports.updateE2E = function(channel, client, peer, ccName, functionName, args) {
    return Promise.resolve().then(()=>{
        txIdObject = client.newTransactionID();
        const proposalRequest = {
            chaincodeId: ccName,
            fcn: functionName,
            args: args,
            txId: txIdObject,
        }
        return channel.sendTransactionProposal(proposalRequest);

    }).then((results)=>{
        var proposalResponses = results[0];
        var proposal = results[1];
        if (proposalResponses && proposalResponses[0].response && 
            proposalResponses[0].response.status === 200){
            // console.log('Transaction proposal was good');
        } else {
            console.log(proposalResponses)
            throw new Error('Invalid Proposal');
        }

        var request = { proposalResponses: proposalResponses, proposal: proposal };
        return channel.sendTransaction(request);
    }).then(()=>{
        let txIdStr = txIdObject.getTransactionID();
        return txIdStr
    });
}