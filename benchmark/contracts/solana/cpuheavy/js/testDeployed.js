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

const USIZE_BYTES = 8;
const U128_BITS = 128;

const connection = new Connection('http://localhost:8899/');
// const connection = new Connection('https://api.devnet.solana.com');

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

const getRandomSignature = () => {
    const bits = new BN('128');
    const two = new BN('2');
    const num1000 = new BN('1000');

    const random_seed = new BN(Math.random() * 1000);
    const max_u128 = two.pow(bits);

    const signature = max_u128.mul(random_seed).div(num1000);
    return new Uint8Array(signature.toArray('le', U128_BITS / 8));
}

const parseArguments = () => {
    let size;
    const arguments = process.argv.slice(2);
    const programId = new PublicKey(arguments[0]);

    if (arguments.length === 2) {
        size = parseInt(arguments[1]);
    }else {
        size = 1000;
    }

    const sizeArray = new Uint8Array(new BN(size).toArray('le', 8));
    const signature = getRandomSignature();

    const instruction_data = new Uint8Array([...sizeArray, ...signature]);
    return {
        'programId': programId,
        'sizeArray': size,
        'size': size,
        'instruction_data': instruction_data,
    };
}

const sortIx = (feePayer, dataAccount, arguments) => {
    return new TransactionInstruction({
        keys: [
            { pubkey: dataAccount.publicKey, isSigner: false, isWritable: true },
        ],
        data: arguments.instruction_data,
        programId: arguments.programId,
    });
}

const showTxLogs = async (txId) => {
    connection.getTransaction(txId, {
        commitment: "confirmed",
    }).then((txInfo) => {
        let logs = txInfo.meta.logMessages.map((line) => {
            if (line.startsWith('Program log:')) {
                return line.split(' ').slice(2).join(' ');
            } else {
                return ''
            }
        }).join('\n');
        console.log(logs);
    });
}

const createDataAccountIx = async (feePayer, dataAccount, programId, size) => {
    return SystemProgram.createAccount({
        fromPubkey: feePayer.publicKey,
        newAccountPubkey: dataAccount.publicKey,
        lamports: await connection.getMinimumBalanceForRentExemption(size * 8),
        space: size * 8,
        programId: programId
    });
}

const main = async () => {
    const parsed = parseArguments();
    const dataAccount = new Keypair();

    const feePayer = readFromFile('feePayer');
    const signers = [feePayer, dataAccount];

    const tx = new Transaction()
        .add(await createDataAccountIx(feePayer, dataAccount, parsed.programId, parsed.size))
        .add(sortIx(feePayer, dataAccount, parsed));
    const txId = await sendAndConfirmTransaction(connection, tx, signers, {
        skipPreflight: true,
        preflightCommitment: 'confirmed',
        commitment: 'confirmed',
    });

    console.log('TX:', txId);
    await showTxLogs(txId);
}
main()
    .then(() => {
    })
    .catch((e) => {
        console.log('Error', e);
    })