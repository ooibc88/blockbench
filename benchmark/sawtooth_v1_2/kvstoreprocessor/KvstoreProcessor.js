const { TransactionProcessor } = require('sawtooth-sdk/processor');
const KvstoreHandler = require('./KvstoreHandler');

console.log(process.argv.length)

if (process.argv.length < 3) {
  console.log('missing a validator address')
  process.exit(1)
}

const address = process.argv[2]
console.log(address)
const transactionProcesssor = new TransactionProcessor(address);
transactionProcesssor.addHandler(new KvstoreHandler()); //our Handler class is added here
console.log("add handler")
transactionProcesssor.start(); //our transaction processsor will start listening for incoming transactions.