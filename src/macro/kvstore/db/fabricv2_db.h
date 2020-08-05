#ifndef BLOCKBENCH_FABRICV2_DB_H_
#define BLOCKBENCH_FABRICV2_DB_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/properties.h"
#include "core/timer.h"
#include "core/utils.h"
#include "core/db.h"
#include "core/bb_utils.h"
#include "core/fabricv2_utils.h"

namespace ycsbc {

class FabricV2DB : public DB {
 public:
  FabricV2DB(const std::string &endpoints, const std::string &wl_name);

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

  std::string endpoint_;
  std::unordered_map<std::string, double> *pendingtx_;
  SpinLock *txlock_;

  BBUtils::SmartContractType sctype_;
};

}  // ycsbc

#endif  // BLOCKBENCH_FABRICV2_DB_H_
