var http = require('http');

var post_data = JSON.stringify({
  "jsonrpc": "2.0",
  "method": "deploy",
  "params": {
    "type": 1,
    "chaincodeID":{
      "path":"https://github.com/ijingo/chaincode-test/analytic"
    },
    "ctorMsg": {
      "function":"Init",
      "args":[process.argv[2]]
    }
  },
  "id": 1
});

console.log(post_data);

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
    console.log('STATUS: ' + res.statusCode);
    console.log('HEADERS: ' + JSON.stringify(res.headers));
    res.setEncoding('utf8');
    res.on('data', function (chunk) {
        console.log('Response: ', chunk);
    });
});

post_req.on('error', function(e) {
    console.log('problem with request: ' + e.message);
});

post_req.write(post_data);
post_req.end();
