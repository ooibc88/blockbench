#include "db/hyperledger_db.h"

using namespace std;
using namespace BBUtils::HLUtils;

namespace ycsbc {

HyperLedgerDB::HyperLedgerDB(const string &endpoint, const string &wl_name)
    : endpoint_(endpoint) {
  if (wl_name == "ycsb") {
    sctype_ = BBUtils::SmartContractType::KVStore;
  } else {
    sctype_ = BBUtils::SmartContractType::DoNothing;
  }
  deploy_chain_code(endpoint, sctype_);
}

// ignore table
// ignore field
// read value indicated by a key
int HyperLedgerDB::Read(const string &table, const string &key,
                        const vector<string> *fields, vector<KVPair> &result) {
  return DB::kOK;
}

// ignore table
// update value indicated by a key
int HyperLedgerDB::Update(const string &table, const string &key,
                          vector<KVPair> &values) {
  string val = "";
  for (auto v : values) {
    val += v.first + "=" + v.second + " ";
  }
  for (auto &x : val) {
    if (x == '\"') x = ' ';
  }

  std::string txn_hash = (sctype_ == BBUtils::SmartContractType::DoNothing)
                             ? submit_do_nothing_txn(endpoint_)
                             : submit_set_txn(endpoint_, key, val);
  txlock_->lock();
  (*pendingtx_)[txn_hash] = utils::time_now();
  txlock_->unlock();
  return DB::kOK;
}

// ignore table
// ignore field
// concate values in KVPairs into one long value
int HyperLedgerDB::Insert(const string &table, const string &key,
                          vector<KVPair> &values) {
  return Update(table, key, values);
}

// ignore table
// delete value indicated by a key
int HyperLedgerDB::Delete(const string &table, const string &key) {
  vector<KVPair> empty_val;
  return Update(table, key, empty_val);
}

unsigned int HyperLedgerDB::GetTip() { return get_tip_block_number(endpoint_); }

// get all tx from the start_block until latest
vector<string> HyperLedgerDB::PollTxn(int block_number) {
  return poll_txs_by_block_number(endpoint_, block_number);
}

}  // ycsbc
