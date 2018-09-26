var http = require('http');

var chainCodeID = "4caae8cf2298bbcc8d3e1f8ef57ea1c7f0f78d1b6b96a93822666c8a9d39c3ddb97f3ea36a4b79ac303a1b9296510450a2516b75b0cc86183b7141a593d004bf";
if (process.argv.length >= 6) {
  chainCodeID = process.argv[5];
}

function gen_acc(n) {
  var zeros = "00000000000000000000"
  var str = n.toString(16);
  return "0x"+zeros.slice(str.length)+str;
}

var post_data = JSON.stringify({
  "jsonrpc": "2.0",
  "method": "query",
  "params": {
    "type": 1,
    "chaincodeID":{
	"name" : chainCodeID
    },
    "ctorMsg": {
      "function":"QueryAccount",
      "args":[gen_acc(process.argv[2]), process.argv[3], process.argv[4]]
    }
  },
  "id": 3
});

var timestamp = new Date().getTime();
var post_options = {
  hostname: 'localhost',
  port    : '7050',
  path    : '/chaincode',
  method  : 'POST',
  headers : {
    'Content-Type': 'application/json',
    'Content-Length': post_data.length
  }
};

var post_req = http.request(post_options, function (res) {
  res.setEncoding('utf8');
  res.on('data', function (chunk) {
    console.log('Response: ', chunk);
    console.log('Latency: ', (new Date().getTime()-timestamp)/1000, " sec");
    process.exit();
  });
});

post_req.on('error', function(e) {
  console.log('problem with request: ' + e.message);
});

post_req.write(post_data);
post_req.end();
