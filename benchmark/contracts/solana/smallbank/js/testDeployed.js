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

function writeKeyPairToFile(filename, account) {
    let key;
    try {
        key = Uint8Array.from(account.toBuffer());
    }catch {
        key = account.secretKey;
    }

    let stringContent = key.join(', ');
    fs.writeFile(`./${filename}.txt`, `[${stringContent}]`, () => {});
}


const parseArguments = () => {
    let cmd = {};
    let instruction_data;
    const arguments = process.argv.slice(2);
    cmd['programId'] = new PublicKey(arguments[0]);

    switch (arguments[1]) {
        case 'init': {
            cmd['id'] = -1;
            cmd['accountSize'] = parseInt(arguments[2]);
            break;
        }
        case 'alma': {
            cmd['id'] = 0;
            let ix = new Uint8Array([0]);
            instruction_data = new TextEncoder().encode(`${arguments[2]} ${arguments[3]}`);
            cmd['instruction_data'] = new Uint8Array([...ix, ...instruction_data]);
            break;
        }
        case 'getBalance': {
            cmd['id'] = 1;
            let ix = new Uint8Array([1]);
            instruction_data = new TextEncoder().encode(`${arguments[2]}`);
            cmd['instruction_data'] = new Uint8Array([...ix, ...instruction_data]);
            break;
        }
        case 'updateBalance': {
            cmd['id'] = 2;
            let ix = new Uint8Array([2]);
            instruction_data = new TextEncoder().encode(`${arguments[2]} ${arguments[3]}`);
            cmd['instruction_data'] = new Uint8Array([...ix, ...instruction_data]);
            break;
        }
        case 'updateSaving': {
            cmd['id'] = 3;
            let ix = new Uint8Array([3]);
            instruction_data = new TextEncoder().encode(`${arguments[2]} ${arguments[3]}`);
            cmd['instruction_data'] = new Uint8Array([...ix, ...instruction_data]);
            break;
        }
        case 'sendPayment': {
            cmd['id'] = 4;
            let ix = new Uint8Array([4]);
            instruction_data = new TextEncoder().encode(`${arguments[2]} ${arguments[3]} ${arguments[4]}`);
            cmd['instruction_data'] = new Uint8Array([...ix, ...instruction_data]);
            break;
        }
        case 'writeCheck': {
            cmd['id'] = 5;
            let ix = new Uint8Array([5]);
            instruction_data = new TextEncoder().encode(`${arguments[2]} ${arguments[3]}`);
            cmd['instruction_data'] = new Uint8Array([...ix, ...instruction_data]);
            break;
        }
    }

    return cmd;
}

const createDataAccountIx = async (programId, feePayer, dataAccount, accountSize) => {
    return SystemProgram.createAccount({
        fromPubkey: feePayer.publicKey,
        newAccountPubkey: dataAccount.publicKey,
        lamports: await connection.getMinimumBalanceForRentExemption(accountSize),
        space: accountSize,
        programId: programId,
    });
}

const noWriteIx = (dataAccount, instruction_data, programId) => {
    return new TransactionInstruction({
        keys: [
            { pubkey: dataAccount.publicKey, isSigner: false, isWritable: false },
        ],
        data: instruction_data,
        programId: programId,
    });
}

const writeIx = (dataAccount, instruction_data, programId) => {
    return new TransactionInstruction({
        keys: [
            { pubkey: dataAccount.publicKey, isSigner: false, isWritable: true },
        ],
        data: instruction_data,
        programId: programId,
    });
}

const main = async () => {
    let dataAccount;
    const parsed = parseArguments();
    const feePayer = readFromFile('feePayer');
    const tx = new Transaction();
    let signers = [feePayer];

    if (parsed.id === -1) {
        dataAccount = new Keypair();
        writeKeyPairToFile('dataAccount', dataAccount);

        const createIx = await createDataAccountIx(parsed.programId, feePayer, dataAccount, parsed.accountSize);
        tx.add(createIx);
        signers.push(dataAccount);
    } else if ([0, 2, 3, 4, 5].includes(parsed.id)){
        dataAccount = readFromFile('dataAccount.txt');
        tx.add(writeIx(dataAccount, parsed.instruction_data, parsed.programId));
    } else if (parsed.id === 1) {
        dataAccount = readFromFile('dataAccount.txt');
        tx.add(noWriteIx(dataAccount, parsed.instruction_data, parsed.programId));
    }

    const txId = await sendAndConfirmTransaction(connection, tx, signers, {
        preflightCommitment: "confirmed",
        skipPreflight: true,
        commitment: "confirmed",
    });

    console.log(`TX: ${txId}`);

    if (parsed.id > -1) {
        connection.getTransaction(txId, {
            commitment: "confirmed",
        }).then((txInfo) => {
            let storage = txInfo.meta.logMessages[1].split(' ').slice(2).join(' ');
            let saving = txInfo.meta.logMessages[2].split(' ').slice(2).join('').slice(1, -1).split(',').join('\n');
            let checking = txInfo.meta.logMessages[3].split(' ').slice(2).join('').slice(1, -1).split(',').join('\n');

            console.log(storage);
            console.log('SAVING');
            console.log(saving);
            console.log();
            console.log();
            console.log('CHECKING');
            console.log(checking);
        });

    }



}

main()
    .then(() => {})
    .catch((e) => {
        console.log(e);
    });