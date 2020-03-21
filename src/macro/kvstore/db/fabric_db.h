#ifndef BLOCKBENCH_FABRIC_DB_H_
#define BLOCKBENCH_FABRIC_DB_H_

#include <iostream>
#include <string>
#include <unordered_map>

#include "core/properties.h"
#include "core/timer.h"
#include "core/utils.h"
#include "core/db.h"
#include "core/bb_utils.h"

namespace ycsbc {

class FabricDB : public DB {
 public:
  FabricDB(const std::string &endpoint, const std::string &wl_name);

  void Init(std::unordered_map<std::string, double> *pendingtx,
            SpinLock *lock) {
    pendingtx_ = pendingtx;
    txlock_ = lock;
  }
  int Read(const std::string &table, const std::string &key,
           const std::vector<std::string> *fields, std::vector<KVPair> &result);

  // no scan operation support
  int Scan(const std::string &table, const std::string &key, int len,
           const std::vector<std::string> *fields,
           std::vector<std::vector<KVPair>> &result) {
    return DB::kOK;
  }

  int Update(const std::string &table, const std::string &key,
             std::vector<KVPair> &values);

  int Insert(const std::string &table, const std::string &key,
             std::vector<KVPair> &values);

  int Delete(const std::string &table, const std::string &key);

  int GetTip();

  std::vector<std::string> PollTxn(int block_number);

 private:
  inline void addresses(std::string* ordererAddr, std::string* peerAddr) {
    // std::cout << "endpoint: " << this->endpoint_ << std::endl;
    auto pos = this->endpoint_.find(",");
    *ordererAddr = this->endpoint_.substr(0, pos);
    *peerAddr = this->endpoint_.substr(pos+1);
  }

  std::string endpoint_;
  std::unordered_map<std::string, double> *pendingtx_;
  SpinLock *txlock_;

  BBUtils::SmartContractType sctype_;
};

}  // ycsbc

#endif  // BLOCKBENCH_FABRIC_DB_H_
