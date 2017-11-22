#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <future>
#include <atomic>
#include <sstream>
#include <fstream>

#include "api_adapters/DB.h"
#include "api_adapters/SmallBank.h"
#include "api_adapters/EVMDB.h"
#include "utils/generators.h"
#include "utils/timer.h"
#include "utils/statistic.h"
#include "utils/utils.h"
#include "utils/properties.h"
//#define IVL 100

using namespace std;

atomic<unsigned long long> latency(0);
std::atomic<unsigned long long> latency_interval(0);
std::atomic<unsigned long long> ops(0);
std::ofstream os_;
Timer<double> stat_timer;

const int HL_CONFIRM_BLOCK_LENGTH = 1;
const int BLOCK_POLLING_INTERVAL = 2; 
const int CONFIRM_BLOCK_LENGTH = 5;
const int PARITY_CONFIRM_BLOCK_LENGTH = 1;

void UsageMessage(const char *command);
bool StrStartWith(const char *str, const char *pre);
string ParseCommandLine(int argc, const char *argv[], utils::Properties &props);

SpinLock spinlock_, txlock_;
std::unordered_map<string, double> pendingtx; 

void ClientThread(DB* sb, const int num_ops, const int txrate) {
  UniformGenerator op_gen(1, 6);
  UniformGenerator acc_gen(1, 100000);
  UniformGenerator bal_gen(1, 10);
  double tx_sleep_time = 1.0 / txrate;
  for (int i = 0; i < num_ops; ++i) {
    auto op = op_gen.Next();
    switch (op) {
      case 1:
        sb->Amalgate(acc_gen.Next(), acc_gen.Next());
        break;
      case 2:
        sb->GetBalance(acc_gen.Next());
        break;
      case 3:
        // sb->UpdateBalance(acc_gen.Next(), bal_gen.Next());
        sb->UpdateBalance(acc_gen.Next(), 0);
        break;
      case 4:
        sb->UpdateSaving(acc_gen.Next(), 0);
        break;
      case 5:
        sb->SendPayment(acc_gen.Next(), acc_gen.Next(), 0);
        break;
      case 6:
        sb->WriteCheck(acc_gen.Next(), 0);
        break;
    }
    utils::sleep(tx_sleep_time);
    //sleep(0.1); 
  }
}

int StatusThread(DB* sb, string dbname, string endpoint, double interval, int start_block_height){
  int cur_block_height = start_block_height;
  long start_time;
  long end_time;
  int txcount = 0;
  long latency;
  int confirm_duration = 1;
  if (dbname == "ethereum")
    confirm_duration = CONFIRM_BLOCK_LENGTH;
  else if (dbname == "parity")
    confirm_duration = PARITY_CONFIRM_BLOCK_LENGTH;
  else
    confirm_duration = HL_CONFIRM_BLOCK_LENGTH;

  while(true){
    start_time = time_now(); 
    int tip = sb->get_tip_block_number(); 
    if (tip==-1) // fail
      sleep(interval); 
    while (cur_block_height + confirm_duration <= tip) {      
      vector<string> txs = sb->poll_tx(cur_block_height); 
      cout << "polled block " << cur_block_height << " : " << txs.size() 
           << " txs " << endl; 
      cur_block_height++;           
      long block_time = time_now(); 
      txlock_.lock();
      for (string tmp : txs){ 
        string s = (dbname == "ethereum" || dbname == "parity")
                      ? tmp.substr(1, tmp.length() - 2)  // get rid of ""
                       : tmp; 
        if (pendingtx.find(s)!=pendingtx.end()){ 
          txcount++; 
          latency += (block_time - pendingtx[s]); 
          // then remove
          pendingtx.erase(s); 
        }
      }
      txlock_.unlock(); 
    }
    cout << "In the last "<< interval <<"s, tx count = " << txcount
         << " latency = " << latency/1000000000.0 
        << " outstanding request = " << pendingtx.size() << endl;  
    txcount = 0; 
    latency = 0; 

    end_time = time_now(); 

    //sleep in nanosecond
    utils::sleep(interval - (end_time - start_time) / 1000000000.0);
  }
return 0;
}

DB* CreateDB(std::string dbname, std::string endpoint) {
  if (dbname == "hyperledger") {
    return SmallBank::GetInstance("SmallbankExample", endpoint); 
  } else if (dbname == "ethereum" || dbname == "parity") {
    return EVMDB::GetInstance(dbname, endpoint); 
  } else {
    return NULL;
  }
}

int main(const int argc, const char* argv[]) {
  utils::Properties props;
  string filename = ParseCommandLine(argc, argv, props);

  DB* sb = CreateDB(props["dbname"], props["endpoint"]); 
  if (!sb) {
    cout << "Unknown database name " << props["dbname"] << endl;
    exit(0);  
  }
  if (os_.is_open()) {
    os_.close();
  }
  os_.open(props.GetProperty("file_path", "stat.txt"), std::ios::app);

  int current_tip = sb->get_tip_block_number();
  cout << "Current TIP = " << current_tip << endl;
  sb->Init(&pendingtx, &txlock_); 

  const int thread_num = stoi(props.GetProperty("threadcount", "1"));
  const int txrate = stoi(props.GetProperty("txrate", "10"));
  const int total_ops = stoi(props.GetProperty("total_ops", "10000")); 
  vector<thread> threads;

  Timer<double> timer;
  timer.Start();
  stat_timer.Tic();

  for (int i = 0; i < thread_num; ++i) {
    threads.emplace_back(ClientThread, sb, total_ops / thread_num, txrate);
  }
  threads.emplace_back(StatusThread, sb, props["dbname"], props["endpoint"], BLOCK_POLLING_INTERVAL, current_tip);


  for (auto& th : threads) th.join();

  double duration = timer.End();
  double l = latency.load() / 1000000.0;

  cerr << "# Transaction throughput (KTPS)" << endl;
  cerr << total_ops / duration / 1000 << endl;
  cerr << endl
       << "Avg latency: " << l / total_ops << " sec" << endl;
  if (os_.is_open()) os_.close();
  return 0;
}

string ParseCommandLine(int argc, const char *argv[],
                        utils::Properties &props) {
  int argindex = 1;
  string filename;
  while (argindex < argc && StrStartWith(argv[argindex], "-")) {
    if (strcmp(argv[argindex], "-threads") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("threadcount", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-db") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("dbname", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-endpoint") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("endpoint", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-txrate") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("txrate", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-wl") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("workload", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-wt") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("deploy_wait", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-ops") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("total_ops", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-fp") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("file_path", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-P") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      filename.assign(argv[argindex]);
      ifstream input(argv[argindex]);
      try {
        props.Load(input);
      } catch (const string &message) {
        cout << message << endl;
        exit(0);
      }
      input.close();
      argindex++;
    } else {
      cout << "Unknown option '" << argv[argindex] << "'" << endl;
      exit(0);
    }
  }

  if (argindex == 1 || argindex != argc) {
    UsageMessage(argv[0]);
    exit(0);
  }

  return filename;
}

void UsageMessage(const char *command) {
  cout << "Usage: " << command << " [options]" << endl;
  cout << "Options:" << endl;
  cout << "  -threads n: execute using n threads (default: 1)" << endl;
  cout << "  -wt deploytime: waiting time in second before start to submit "
          "transactions for deployment the smart "
          "contract/chaincode" << endl;
  cout << "  -db dbname: specify the name of the DB to use (e.g., hyperledger)"
       << endl;
  //cout << "  -wl workload: specify the type of smart contract to run (choices: "
  //        "ycsb, donothing, smallbank. By default: donothing)" << endl;
  cout << "  -P propertyfile: load properties from the given file. Multiple "
          "files can" << endl;
  cout << "                   be specified, and will be processed in the order "
          "specified" << endl;
  cerr << "   eg: " << "./driver"
         << " -ops 10000 -threads 4 -txrate 10 -fp stat.txt -endpoint 127.0.0.1:8000 -db ethereum" << endl;
}

inline bool StrStartWith(const char *str, const char *pre) {
  return strncmp(str, pre, strlen(pre)) == 0;
}
