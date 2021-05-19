const { TransactionProcessor } = require('sawtooth-sdk/processor');
const KvstoreHandler = require('./KvstoreHandler');



const address = 'tcp://validator:4004'; //address of the validator from the docker-compose file
const transactionProcesssor = new TransactionProcessor(address);
transactionProcesssor.addHandler(new KvstoreHandler()); //our Handler class is added here
console.log("add handler")
transactionProcesssor.start(); //our transaction processsor will start listening for incoming transactions.