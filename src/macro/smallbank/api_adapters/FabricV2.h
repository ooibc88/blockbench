#ifndef SMARTCONTRACT_DRIVERS_FABRICV2_H_
#define SMARTCONTRACT_DRIVERS_FABRICV2_H_

#include <string>
#include "DB.h"
#include "utils/timer.h"
#include "utils/utils.h"
#include "utils/spinlock.h"

#include <unordered_map>
#include <vector>
using std::unordered_map; 
using std::string; 
using std::vector; 

class FabricV2 : public DB {
 public:
  void Amalgate(unsigned acc1, unsigned acc2);
  void GetBalance(unsigned acc);
  void UpdateBalance(unsigned acc, unsigned amount);
  void UpdateSaving(unsigned acc, unsigned amount);
  void SendPayment(unsigned acc1, unsigned acc2, unsigned amount);
  void WriteCheck(unsigned acc, unsigned amount);
  void deploy(const std::string& path, const std::string& endpoint);

  static FabricV2* GetInstance(std::string path, std::string endpoint) {
    static FabricV2 fabric;
    // fabric.deploy(path, endpoint);
    fabric.endpoint_ = endpoint;
    return &fabric;
  }

  FabricV2() {}
  FabricV2(std::string path, std::string endpoint) {
    deploy(path, endpoint); 
  }

  void Init(unordered_map<string, double> *pendingtx, SpinLock *lock){
    pendingtx_ = pendingtx;
    txlock_ = lock;
  }

  // ~FabricV2() {}

  int get_tip_block_number();
  vector<string> poll_tx(int block_number);
 private:
  void InvokeWithOneArg(std::string funcName, std::string arg);
  void InvokeWithTwoArgs(std::string funcName, std::string arg1, std::string arg2);
  void InvokeWithThreeArgs(std::string funcName, std::string arg1, 
                                  std::string arg2, std::string arg3);

  std::string randomTxnServiceAddr();
  void add_to_queue(string json); 

  inline void addresses(std::string* blkServiceAddr, std::vector<std::string>* txnServiceAddrs) {
    // std::cout << "endpoint: " << this->endpoint_ << std::endl;
    auto pos = this->endpoint_.find(",");
    *blkServiceAddr = this->endpoint_.substr(0, pos);
    if (txnServiceAddrs != NULL)  {
      std::string remains = this->endpoint_.substr(pos+1);
      std::string delimiter = ",";

      size_t pos = 0;
      while ((pos = remains.find(delimiter)) != std::string::npos) {
          std::string txnServiceAddr = remains.substr(0, pos);
          txnServiceAddrs->push_back(txnServiceAddr);
          remains.erase(0, pos + delimiter.length());
      }
      txnServiceAddrs->push_back(remains);
    }
  }


  std::string chaincode_name_, endpoint_;
  unordered_map<string, double> *pendingtx_; 
  SpinLock *txlock_; 
};

#endif
