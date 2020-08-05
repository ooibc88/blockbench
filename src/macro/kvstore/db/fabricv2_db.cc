#include "db/fabricv2_db.h"
#include <time.h>

using namespace std;
using namespace BBUtils::FABV2Utils;

namespace ycsbc {

FabricV2DB::FabricV2DB(const string &endpoint, const string &wl_name)
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
int FabricV2DB::Read(const string &table, const string &key,
                        const vector<string> *fields, vector<KVPair> &result) {
  return DB::kOK;
}

// ignore table
// update value indicated by a key
int FabricV2DB::Update(const string &table, const string &key,
                          vector<KVPair> &values) {
  string val = "";
  for (auto v : values) {
    val += v.first + "=" + v.second + " ";
  }
  for (auto &x : val) {
    if (x == '\"') x = ' ';
  }
  std::string blkServiceAddr;
  std::vector<std::string> txnServiceAddrs;
  this->addresses(&blkServiceAddr, &txnServiceAddrs);
  srand(time(NULL));
  size_t i = std::rand() % txnServiceAddrs.size();

  std::string txn_hash = (sctype_ == BBUtils::SmartContractType::DoNothing)
                             ? submit_do_nothing_txn(txnServiceAddrs[i])
                             : submit_set_txn(txnServiceAddrs[i], key, val);
  txlock_->lock();
  (*pendingtx_)[txn_hash] = utils::time_now();
  txlock_->unlock();
  return DB::kOK;
}

// ignore table
// ignore field
// concate values in KVPairs into one long value
int FabricV2DB::Insert(const string &table, const string &key,
                          vector<KVPair> &values) {
  return Update(table, key, values);
}

// ignore table
// delete value indicated by a key
int FabricV2DB::Delete(const string &table, const string &key) {
  vector<KVPair> empty_val;
  return Update(table, key, empty_val);
}

int FabricV2DB::GetTip() { 
  std::string blkServiceAddr;
  this->addresses(&blkServiceAddr, NULL);
  return get_tip_block_number(blkServiceAddr); 
}

// get all tx from the start_block until latest
vector<string> FabricV2DB::PollTxn(int block_number) {
  std::string blkServiceAddr;
  this->addresses(&blkServiceAddr, NULL);
  return poll_txs_by_block_number(blkServiceAddr, block_number);
}

}  // ycsbc
