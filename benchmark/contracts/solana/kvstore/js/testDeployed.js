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

const dataAccountSize = 1000;
const connection = new Connection('http://localhost:8899/');

const parseArguments = () => {
    let instruction_tag, input_slice, dataAccount;
    const arguments = process.argv.slice(2);
    const programId = new PublicKey(arguments[0]);

    if (arguments.length === 2) {
        return {
            'instruction_data': [0],
            'programId': programId,
        }
    }

    switch (arguments[2]) {
        case 'set': {
            if (arguments.length !== 5) {
                throw 'Did not provide key and value';
            }
            dataAccount = new PublicKey(arguments[1]);
            instruction_tag = new Uint8Array([1]);
            input_slice = new TextEncoder().encode(`${arguments[3]} ${arguments[4]}`);
            break;
        }
        case 'get': {
            if (arguments.length !== 4) {
                throw 'Did not provide key';
            }
            dataAccount = new PublicKey(arguments[1]);
            instruction_tag = new Uint8Array([2]);
            input_slice = new TextEncoder().encode(arguments[3]);
            break;
        }
        default: {
            throw `Invalid command: ${arguments[1]}`;
        }
    }
    return {
        'programId': programId,
        'dataAccount': dataAccount,
        'instruction_data': new Uint8Array([...instruction_tag, ...input_slice]),
    }
}

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

const createDataAccount = async (feePayer, dataAccount, size, programId) => {
    return SystemProgram.createAccount({
        fromPubkey: feePayer.publicKey,
        newAccountPubkey: dataAccount.publicKey,
        lamports: await connection.getMinimumBalanceForRentExemption(dataAccountSize),
        space: dataAccountSize,
        programId: programId
    });
}

const executeIx = async (feePayer, dataAccount, instruction_data, programId) => {
    return new TransactionInstruction({
        keys: [
            { pubkey: dataAccount, isSigner: false, isWritable: true },
        ],
        data: instruction_data,
        programId: programId,
    });
}

const main = async () => {
    const parsed = parseArguments();
    const feePayer = readFromFile('feePayer');
    const dataAccount = new Keypair();

    const tx = new Transaction();
    let signers = [feePayer];

    if (parsed.instruction_data[0] === 0) {
        tx.add(await createDataAccount(feePayer, dataAccount, parsed.programId));
        signers.push(dataAccount);
        console.log(`Data Account: ${dataAccount.publicKey}`);
    }else {
        tx.add(await executeIx(feePayer, parsed.dataAccount, parsed.instruction_data, parsed.programId));
    }

    const txId = await sendAndConfirmTransaction(connection, tx, signers, {
        skipPreflight: true,
        commitment: "confirmed",
        preflightCommitment: "confirmed",
    });
    console.log(`TX: ${txId}`);

    if (parsed.instruction_data[0] === 2) {
        const txInfo = await connection.getTransaction(txId, {
            commitment: "confirmed",
        });
        console.log(txInfo.meta.logMessages[1].split(' ').slice(2).join(' '));
    }
}

main()
    .then(() => {
        console.log('Success');
    })
    .catch((e) => {
        console.log('Error', e);
    })