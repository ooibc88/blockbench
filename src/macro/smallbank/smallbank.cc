#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <future>
#include <atomic>
#include <sstream>
#include <fstream>
#include "api_adapters/smallbank_api.h"
#include "utils/generators.h"
#include "utils/timer.h"
#include "utils/statistic.h"
#include "utils/utils.h"

//#define IVL 100

using namespace std;

atomic<unsigned long long> latency(0);
std::atomic<unsigned long long> latency_interval(0);
std::atomic<unsigned long long> ops(0);
std::ofstream os_;
Timer<double> stat_timer;

const int HL_CONFIRM_BLOCK_LENGTH = 1;
const int BLOCK_POLLING_INTEVAL = 2; 

SpinLock spinlock_, txlock_;
std::unordered_map<string, double> pendingtx; 

void ClientThread(SmallBank* sb, const int num_ops, const int ivl) {
  UniformGenerator op_gen(1, 6);
  UniformGenerator acc_gen(1, 100000);
  UniformGenerator bal_gen(1, 10);
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
    sleep(0.1); 
  }
}

int StatusThread(SmallBank* sb, string endpoint, double interval, int start_block_height){
  int cur_block_height = start_block_height;
  long start_time;
  long end_time;
  int txcount = 0;
  long latency;

  while(true){
    start_time = time_now(); 
    int tip = sb->get_tip_block_number(); 
    if (tip==-1) // fail
      sleep(interval); 
    while (cur_block_height+HL_CONFIRM_BLOCK_LENGTH<= tip){      
      vector<string> txs = sb->poll_tx(cur_block_height); 
      cout << "polled block " << cur_block_height << " : " << txs.size() << " txs " << endl; 
      cur_block_height++;           
      long block_time = time_now(); 
      txlock_.lock();
      for (string tmp : txs){
        string s = tmp; 
        if (pendingtx.find(s)!=pendingtx.end()){ 
          txcount++; 
          latency += (block_time - pendingtx[s]); 
          // then remove
          pendingtx.erase(s); 
        }
      }
      txlock_.unlock(); 
    }
    cout << "In the last "<<interval<<"s, tx count = " << txcount << " latency = " << latency/1000000000.0 << " outstanding request = " << pendingtx.size() << endl;  
    txcount = 0; 
    latency = 0; 

    end_time = time_now(); 

    //sleep in nanosecond
    sleep(interval-(end_time-start_time)/1000000000.0);
  }

}

int main(int argc, char* argv[]) {
  if (argc != 6) {
    cerr << "Usage: " << argv[0]
         << " [total_ops] [thread_num] [interval] [file_path] [end_point]"
         << endl;
    cerr << "   eg: " << argv[0]
         << " 10000 4 1000 stat.txt localhost:7050/chaincode/" << endl;
    return 0;
  }

  const int total_ops = stoi(argv[1]);
  const int thread_num = stoi(argv[2]);
  const int ivl = stoi(argv[3]);
  string spath = argv[4];

  if (os_.is_open()) {
    os_.close();
  }
  os_.open(spath, std::ios::app);

  SmallBank* sb = SmallBank::GetInstance("SmallbankExample", argv[5]);
  sb->Init(&pendingtx, &txlock_); 

  int current_tip = sb->get_tip_block_number();
  cout << "Current TIP = " << current_tip << endl;

  vector<thread> threads;

  Timer<double> timer;
  timer.Start();
  stat_timer.Tic();

  for (int i = 0; i < thread_num; ++i) {
    threads.emplace_back(ClientThread, sb, total_ops / thread_num, ivl);
  }
  threads.emplace_back(StatusThread, sb, argv[5], BLOCK_POLLING_INTEVAL, current_tip);


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
