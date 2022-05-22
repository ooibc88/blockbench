const fs = require('fs');
const {
    Connection,
    sendAndConfirmTransaction,
    Keypair,
    Transaction,
    TransactionInstruction,
    PublicKey,
    SystemProgram,
} = require('@solana/web3.js');
const BN = require('bn.js');
const aux = require('./aux');
const {readFromFile} = require("./aux");

const connection = new Connection('http://localhost:8899/');

const main = async () => {
    let dataAccount;
    const parsed = aux.parseArguments();
    const feePayer = aux.readFromFile('feePayer');
    const tx = new Transaction();
    let signers = [feePayer];

    if (parsed.id === -1) {
        dataAccount = new Keypair();
        aux.writeKeyPairToFile('dataAccount', dataAccount);

        tx.add(await aux.createDataAccount(connection, feePayer, dataAccount, parsed.dataAccountSize, parsed.programId));
        signers.push(dataAccount);
    } else if ([0].includes(parsed.id)) {
        dataAccount = readFromFile('dataAccount');
        tx.add(aux.noWriteIx(dataAccount, parsed.instruction_data, parsed.programId));
    } else {
        dataAccount = readFromFile('dataAccount');
        tx.add(aux.writeIx(dataAccount, parsed.instruction_data, parsed.programId));
    }

    const txId = await sendAndConfirmTransaction(connection, tx, signers, {
       preflightCommitment: "confirmed",
       commitment: "confirmed",
       skipPreflight: true,
    });

    console.log('TX:', txId);

    if (parsed.id !== -1) {
        await aux.showLogs(connection, txId);
    }

}

main()
    .then(() => {})
    .catch((e) => {
        console.log(e);
    });