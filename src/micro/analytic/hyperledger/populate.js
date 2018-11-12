var http = require('http');
const batch_size = 3;
var total_size = parseInt(process.argv[2]);
var chainCodeID = "4caae8cf2298bbcc8d3e1f8ef57ea1c7f0f78d1b6b96a93822666c8a9d39c3ddb97f3ea36a4b79ac303a1b9296510450a2516b75b0cc86183b7141a593d004bf";
if (process.argv.length >= 4) {
  chainCodeID = process.argv[3];
}

var zipfGenerator = require('zipfian').getGenerator(99999);
function gen_acc(n) {
  var zeros = "00000000000000000000"
  var str = n.toString(16);
  return "0x"+zeros.slice(str.length)+str;
}

function poll_block(height) {

  var get_options = {
    hostname: 'localhost',
    port    : '7050',
    path    : '/chain/blocks/'+height,
    method  : 'GET',
    headers : {
      'Content-Type': 'application/json'
    }
  };

  var req = http.request(get_options, function(res) {
    res.setEncoding('utf8');
    res.on('data', function (chunk) {
      console.log(chunk);
    })
  });

  req.on('error', function(e) {
    console.log('problem with request: ' + e.message);
  });

  req.end();
}

function invoke(func, args) {
  var post_data = JSON.stringify({
    "jsonrpc": "2.0",
    "method": "invoke",
    "params": {
      "type": 1,
      "chaincodeID":{
	"name" : chainCodeID
      },
      "ctorMsg": {
        "function": func,
        "args": args
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
  });

  post_req.on('error', function(e) {
    console.log('problem with request: ' + e.message);
  });

  post_req.write(post_data);
  post_req.end();
}

var last_height = -1;
var target_block;

function poll_height() {

  var get_height_options = {
    hostname: 'localhost',
    port    : '7050',
    path    : '/chain',
    method  : 'GET',
    headers : {
      'Content-Type': 'application/json'
    }
  };

  var req = http.request(get_height_options, function (res) {
    // console.log('STATUS: ' + res.statusCode);
    // console.log('HEADERS: ' + JSON.stringify(res.headers));
    res.setEncoding('utf8');
    res.on('data', function (chunk) {
      height = JSON.parse(chunk).height;
      if (height != last_height) {
        var now_time = new Date().getTime();
        //console.log("timestamp: "+ now_time);
        if (last_height != -1) {
          //console.log("Latency: "+ ((now_time-last_time)/1000) + " sec");
          ++target_block;
        } else {
          txn_count = 0;
          target_block = 1;
        }
        //console.log("now hl blockchain tip height: " + (height-1));
        if (txn_count < total_size) {
          for (var i = 0; i < batch_size; ++i) {
            var from = Math.round(zipfGenerator.next());
            var to = Math.round(zipfGenerator.next());
            while(from == to || from > 99999 || to > 99999 || from < 0 || to < 0) {
              from = Math.round(zipfGenerator.next());
              to = Math.round(zipfGenerator.next());
            }
            // console.log(from, " - ",  to);
            var val = Math.floor((Math.random() * 100) + 1);
            invoke("Send", [gen_acc(from), gen_acc(to), val.toString()]);
          }
          invoke("Commit", []);
          if (target_block % 1000 == 0) {
            console.log("commited " + target_block);
          }
          // now_time = new Date().getTime();
          txn_count += batch_size;
        } else {
          process.exit();
        }
        last_height = height;
        last_time = now_time;
      }
    });
  });

  req.on('error', function(e) {
    console.log('problem with request: ' + e.message);
  });

  req.end();
}

setInterval(poll_height, 100);
