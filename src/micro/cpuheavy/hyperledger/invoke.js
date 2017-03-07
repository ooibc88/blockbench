var http = require('http');

var post_data = JSON.stringify({
  "jsonrpc": "2.0",
  "method": "invoke",
  "params": {
    "type": 1,
    "chaincodeID":{
      "name":"3bfaa2ce7321feeb553033b19f0af9ff9ce0a738d7e3df63b9fe1345b8c3b1f9ed8d7af79c99a20fb50a3bcd2b77bf6122ceb7f67f54a473cf9b306d9a905473"
    },
    "ctorMsg": {
      "function":"sort",
      "args":[process.argv[2]]
    }
  },
  "id": 3
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
    console.log("timestamp: "+(new Date().getTime()));
    console.log('Response: ', chunk);
  });
});

post_req.on('error', function(e) {
  console.log('problem with request: ' + e.message);
});

post_req.write(post_data);
post_req.end();
