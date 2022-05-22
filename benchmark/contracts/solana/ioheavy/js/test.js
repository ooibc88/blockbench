const BN = require('bn.js');

const num = process.argv[2];

let ret = new BN(num).toArray('le', 20);

console.log(ret);

