#ifndef BLOCKBENCH_HYPERLEDGER_DB_H_
#define BLOCKBENCH_HYPERLEDGER_DB_H_

#include <iostream>
#include <string>
#include <unordered_map>

#include "core/properties.h"
#include "core/timer.h"
#include "core/utils.h"
#include "core/db.h"
#include "core/hyperledger_utils.h"

namespace ycsbc {

class HyperLedgerDB : public DB {
 public:
  HyperLedgerDB(const std::string &endpoint, const std::string &wl_name);

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

  unsigned int GetTip();

  std::vector<std::string> PollTxn(int block_number);

 private:
  std::string endpoint_;
  std::unordered_map<std::string, double> *pendingtx_;
  SpinLock *txlock_;

  BBUtils::SmartContractType sctype_;
};

}  // ycsbc

#endif  // BLOCKBENCH_HYPERLEDGER_DB_H_
