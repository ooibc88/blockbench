#ifndef BLOCKBENCH_EVMDB_H_
#define BLOCKBENCH_EVMDB_H_

#include <iostream>
#include <string>
#include <unordered_map>

#include "core/evm_utils.h"
#include "core/properties.h"
#include "core/utils.h"
#include "core/timer.h"
#include "core/db.h"

namespace ycsbc {

class EVMDB : public DB {
 public:
  EVMDB(const std::string &endpoint, const std::string &dbname,
        const std::string &wl_name, unsigned deploy_wait_sec);

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
  std::unordered_map<std::string, double> *pendingtx_;
  SpinLock *txlock_;

  std::string from_address_, to_address_, endpoint_;
  std::vector<std::string> poll_tx_by_hash(std::string block_hash);

  BBUtils::EVMUtils::EVMType evmtype_;
  BBUtils::SmartContractType sctype_;
};

}  // ycsbc

#endif  // BLOCKBENCH_EVMDB_H_
