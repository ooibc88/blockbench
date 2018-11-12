var http = require('http');

var start_block = parseInt(process.argv[2])
var end_block = parseInt(process.argv[3])
var max = -1;

var timestamp;

var chainCodeID = "4caae8cf2298bbcc8d3e1f8ef57ea1c7f0f78d1b6b96a93822666c8a9d39c3ddb97f3ea36a4b79ac303a1b9296510450a2516b75b0cc86183b7141a593d004bf";
if (process.argv.length >= 5) {
  chainCodeID = process.argv[4];
}

function get_max(block_num) {
  if (block_num == start_block-1) {
    console.log(max);
    console.log("Latency: "+((new Date().getTime()-timestamp)/1000)+" sec");
    process.exit();
  }
  if (block_num % 1000 == 0) {
    console.log("checking: "+block_num);
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
        "function":"QueryBlock",
        "args":[block_num.toString()]
      }
    },
    "id": 3
  });

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
      ret = JSON.parse(JSON.parse(chunk)["result"]["message"]);
      for (var i in ret) {
        max = max > ret[i]["Val"] ? max : ret[i]["Val"];
      }
      
      get_max(block_num-1);
    });
  });

  post_req.on('error', function(e) {
    console.log('problem with request: ' + e.message);
  });

  post_req.write(post_data);
  post_req.end();
}

timestamp = new Date().getTime();
get_max(end_block);
