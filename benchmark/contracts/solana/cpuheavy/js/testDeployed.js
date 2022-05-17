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

const arguments = process.argv.slice(2);
const cpuHeavyProgram = new PublicKey(arguments[0]);

let size = parseInt(arguments[1]);
if (isNaN(size)) {
    size = 1000;
}
const connection = new Connection('http://localhost:8899/');

const readFromFile = (filename) => {
    let dataWallet = fs.readFileSync(`./${filename}`, 'utf8');
    dataWallet = dataWallet.slice(1, -1);
    dataWallet = new Uint8Array(dataWallet.split(',').map((elem) => parseInt(elem)));

    try {
        return new PublicKey(dataWallet);
    }catch {
        return Keypair.fromSecretKey(dataWallet);
    }
}

const main = new Promise(async (resolve) => {
    // const feePayer = new Keypair();
    // const airdropTx = await connection.requestAirdrop(feePayer.publicKey, LAMPORTS_PER_SOL);
    const feePayer = readFromFile('feePayer');
    const dataAccount = new Keypair();

    const size_array = (new BN(size)).toArray('le', 16);
    const signature = (new BN('10')).toArray('le', 16);

    const instruction_data = new Uint8Array([...size_array, ...signature]);
    const tx = new Transaction();

    const createDataAccountIx = SystemProgram.createAccount({
        fromPubkey: feePayer.publicKey,
        newAccountPubkey: dataAccount.publicKey,
        lamports: await connection.getMinimumBalanceForRentExemption(size * 16),
        space: size * 16,
        programId: cpuHeavyProgram,
    });
    tx.add(createDataAccountIx);

    tx.add(
        new TransactionInstruction({
            keys: [
                { pubkey: dataAccount.publicKey, isSigner: false, feePayer, isWritable: true, },
            ],
            data: instruction_data,
            programId: cpuHeavyProgram,
        },)
    );

    const txId = await sendAndConfirmTransaction(connection, tx, [feePayer, dataAccount], {
        preflightCommitment: "confirmed",
        skipPreflight: true,
        commitment: "confirmed",
    });

    resolve(txId);
});

main
    .then((txId) => {
        console.log('Success', txId);
    })
    .catch((e) => {
        console.log('Error', e);
    })