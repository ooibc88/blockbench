const {
    PublicKey,
    Keypair,
    SystemProgram,
    TransactionInstruction,
} = require("@solana/web3.js");
const fs = require("fs");
const BN = require('bn.js');

const stringToBytes20 = (string) => {
    if (string.length > 20) {
        throw 'Key can be 20 characters at most';
    }
    string = new TextEncoder().encode(string);

    const values_to_pad = 20 - string.length;
    const empty_values = new Uint8Array(values_to_pad);

    return new Uint8Array([...string, ...empty_values]);
}

const parseArguments = () => {
    let cmd = {};
    const arguments = process.argv.slice(2);
    cmd['programId'] = new PublicKey(arguments[0]);

    switch (arguments[1]) {
        case 'init': {
            cmd['id'] = -1;
            cmd['dataAccountSize'] = parseInt(arguments[2]);
            break;
        }
        case 'get': {
            cmd['id'] = 0;
            const ixTag = new Uint8Array([0]);

            cmd['key'] = stringToBytes20(arguments[2]);
            cmd['instruction_data'] = new Uint8Array([
                ...ixTag,
                ...cmd['key'],
            ]);
            break;
        }
        case 'set': {
            cmd['id'] = 1;
            const ixTag = new Uint8Array([1]);
            cmd['key'] = stringToBytes20(arguments[2]);
            cmd['value'] = new BN(arguments[3]).toArray('le', 8);
            cmd['instruction_data'] = new Uint8Array([
                ...ixTag,
                ...cmd['key'],
                ...cmd['value'],
            ])
        }
        case 'write': {
            cmd['id'] = 2;
            const ixTag = new Uint8Array([2]);
            cmd['key'] = stringToBytes20(arguments[2]);
            cmd['value'] = new BN(arguments[3]).toArray('le', 8);
            cmd['instruction_data'] = new Uint8Array([
                ...ixTag,
                ...cmd['key'],
                ...cmd['value'],
            ])
        }
    }
    return cmd;
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

function writeKeyPairToFile(filename, account) {
    let key;
    try {
        key = Uint8Array.from(account.toBuffer());
    }catch {
        key = account.secretKey;
    }

    let stringContent = key.join(', ');
    fs.writeFile(`./${filename}`, `[${stringContent}]`, () => {});
}

const createDataAccount = async (connection, feePayer, dataAccount, dataAccountSize, programId) => {
    return SystemProgram.createAccount({
        fromPubkey: feePayer.publicKey,
        newAccountPubkey: dataAccount.publicKey,
        lamports: await connection.getMinimumBalanceForRentExemption(dataAccountSize),
        space: dataAccountSize,
        programId: programId
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

const showLogs = async (connection, txId) => {
    connection.getTransaction(txId, {
        commitment: "confirmed",
    }).then((txInfo) => {
        const logs = txInfo.meta.logMessages
            .filter((line) => line.startsWith('Program log: '))
            .map((line) => {
                return line.split(' ').slice(2).join(' ');
            })
            .join('\n');
        console.log(logs)
    });
}

module.exports = {
    parseArguments: parseArguments,
    readFromFile: readFromFile,
    writeKeyPairToFile: writeKeyPairToFile,
    createDataAccount: createDataAccount,
    writeIx: writeIx,
    noWriteIx: noWriteIx,
    showLogs: showLogs,
};