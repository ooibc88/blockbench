#ifndef SMARTCONTRACT_DRIVERS_DB_H_
#define SMARTCONTRACT_DRIVERS_DB_H_

#include <string>
#include "utils/timer.h"
#include "utils/utils.h"
#include <unordered_map>
#include <vector>
using std::unordered_map; 
using std::string; 
using std::vector; 

class DB {
 public:
  virtual void Amalgate(unsigned acc1, unsigned acc2)=0;
  virtual void GetBalance(unsigned acc)=0;
  virtual void UpdateBalance(unsigned acc, unsigned amount)=0;
  virtual void UpdateSaving(unsigned acc, unsigned amount)=0;
  virtual void SendPayment(unsigned acc1, unsigned acc2, unsigned amount)=0;
  virtual void WriteCheck(unsigned acc, unsigned amount)=0;
  /*
    static DB* GetInstance(std::string path, std::string endpoint) {
    static DB sb;
    sb.deploy(path, endpoint);
    return &sb;
  }*/

  virtual void Init(unordered_map<string, double> *pendingtx, SpinLock *lock){
    pendingtx_ = pendingtx;
    txlock_ = lock;
  }

  ~DB() {}

  virtual unsigned int get_tip_block_number()=0;
  virtual vector<string> poll_tx(int block_number)=0;
  //virtual int find_tip(string json)=0;
  //virtual vector<string> find_tx(string json)=0; 
  //virtual string get_json_field(const string &json, const string &key)=0; 
 private:
  virtual void deploy(const std::string& path, const std::string& endpoint)=0;
  virtual void add_to_queue(string json)=0; 
  std::string chaincode_name_, endpoint_;
  unordered_map<string, double> *pendingtx_; 
  SpinLock *txlock_; 
};

#endif
