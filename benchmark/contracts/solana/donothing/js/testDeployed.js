const {
    Connection,
    sendAndConfirmTransaction,
    Keypair,
    Transaction,
    LAMPORTS_PER_SOL,
    TransactionInstruction, PublicKey
} = require('@solana/web3.js');

const arguments = process.argv.slice(2);
const doNothingProgram = new PublicKey(arguments[0]);

const connection = new Connection('http://localhost:8899/');

const main = new Promise(async (resolve) => {
    const feePayer = new Keypair();
    const airdropTx = await connection.requestAirdrop(feePayer.publicKey, LAMPORTS_PER_SOL);

    await connection.confirmTransaction(airdropTx);

    const tx = new Transaction().add(
        new TransactionInstruction({
            keys: [
                { pubkey: feePayer.publicKey, isSigner: true, feePayer, isWritable: false, },
            ],
            data: null,
            programId: doNothingProgram
        })
    );

    const txId = await sendAndConfirmTransaction(connection, tx, [feePayer], {
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