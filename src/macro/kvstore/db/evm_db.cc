#include "db/evm_db.h"

#include <thread>
#include <chrono>

using namespace std;
using namespace BBUtils::EVMUtils;

namespace ycsbc {

EVMDB::EVMDB(const string &endpoint, const string &dbname,
             const string &wl_name, unsigned deploy_wait_sec)
    : endpoint_(endpoint),
      evmtype_(dbname == "ethereum" ? EVMType::Ethereum : EVMType::Parity) {
  if (wl_name == "ycsb") {
    sctype_ = BBUtils::SmartContractType::KVStore;
  } else if (wl_name == "smallbank") {
    sctype_ = BBUtils::SmartContractType::SmallBank;
  } else {
    sctype_ = BBUtils::SmartContractType::DoNothing;
  }
  from_address_ = get_from_address(endpoint_);
  if (evmtype_ == EVMType::Parity) unlock_address(endpoint_, from_address_);
  auto receipt = deploy_smart_contract(endpoint_, from_address_, sctype_);
  std::this_thread::sleep_for(std::chrono::seconds(deploy_wait_sec));
  to_address_ = lookup_smart_contract_address_or_die(endpoint_, receipt);
  cout << "to address: " << to_address_ << endl;
  cout << "Smart contract " + wl_name + " deploy ready" << endl;
}

/// ignore table
/// ignore field
/// read value indicated by a key
int EVMDB::Read(const string &table, const string &key,
                const vector<string> *fields, vector<KVPair> &result) {
  if (evmtype_ == EVMType::Parity) unlock_address(endpoint_, from_address_);
  double start_time = utils::time_now();
  std::string txn_hash =
      (sctype_ == BBUtils::SmartContractType::DoNothing)
          ? submit_do_nothing_txn(endpoint_, from_address_, to_address_)
          : submit_get_txn(endpoint_, key, from_address_, to_address_);
  txlock_->lock();
  (*pendingtx_)[txn_hash] = start_time;
  txlock_->unlock();
  return DB::kOK;
}

// ignore table
// update value indicated by a key
int EVMDB::Update(const string &table, const string &key,
                  vector<KVPair> &values) {
  string val = "";
  for (auto v : values) {
    val += v.first + "=" + v.second + " ";
  }
  if (evmtype_ == EVMType::Parity) unlock_address(endpoint_, from_address_);

  double start_time = utils::time_now();
  std::string txn_hash =
      (sctype_ == BBUtils::SmartContractType::DoNothing)
          ? submit_do_nothing_txn(endpoint_, from_address_, to_address_)
          : submit_set_txn(endpoint_, key, val, from_address_, to_address_);
  txlock_->lock();
  (*pendingtx_)[txn_hash] = start_time;
  txlock_->unlock();

  return DB::kOK;
}

// ignore table
// ignore field
// concate values in KVPairs into one long value
int EVMDB::Insert(const string &table, const string &key,
                  vector<KVPair> &values) {
  return Update(table, key, values);
}

// ignore table
// delete value indicated by a key
int EVMDB::Delete(const string &table, const string &key) {
  vector<KVPair> empty_val;
  return Update(table, key, empty_val);
}

// get all tx from the start_block until latest
vector<string> EVMDB::PollTxn(int block_number) {
  return poll_txs_by_block_number(endpoint_, block_number);
}

unsigned int EVMDB::GetTip() { return get_tip_block_number(endpoint_); }
}  // ycsbc
