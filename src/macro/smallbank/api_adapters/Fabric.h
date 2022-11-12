#ifndef SMARTCONTRACT_DRIVERS_FABRIC_H_
#define SMARTCONTRACT_DRIVERS_FABRIC_H_

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

class Fabric : public DB {
 public:
  void Amalgate(unsigned acc1, unsigned acc2);
  void GetBalance(unsigned acc);
  void UpdateBalance(unsigned acc, unsigned amount);
  void UpdateSaving(unsigned acc, unsigned amount);
  void SendPayment(unsigned acc1, unsigned acc2, unsigned amount);
  void WriteCheck(unsigned acc, unsigned amount);
  void deploy(const std::string& path, const std::string& endpoint);

  static Fabric* GetInstance(std::string path, std::string endpoint) {
    static Fabric fabric;
    fabric.deploy(path, endpoint);
    fabric.endpoint_ = endpoint;
    return &fabric;
  }

  Fabric() {}
  Fabric(std::string path, std::string endpoint) {
    deploy(path, endpoint); 
  }

  void Init(unordered_map<string, double> *pendingtx, SpinLock *lock){
    pendingtx_ = pendingtx;
    txlock_ = lock;
  }

  // ~Fabric() {}

  int get_tip_block_number();
  vector<string> poll_tx(int block_number);
 private:
  void add_to_queue(string json); 
  std::string exec(const char* cmd);

  inline void addresses(std::string* ordererAddr, std::string* peerAddr) {
    // std::cout << "endpoint: " << this->endpoint_ << std::endl;
    auto pos = this->endpoint_.find(",");
    *ordererAddr = this->endpoint_.substr(0, pos);
    *peerAddr = this->endpoint_.substr(pos+1);
  }


  std::string chaincode_name_, endpoint_;
  unordered_map<string, double> *pendingtx_; 
  SpinLock *txlock_; 
};

#endif
