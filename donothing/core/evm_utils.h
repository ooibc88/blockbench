#ifndef BLOCKBENCH_EVM_UTILS_H_
#define BLOCKBENCH_EVM_UTILS_H_

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>

#include "bb_utils.h"

namespace BBUtils {
namespace EVMUtils {

enum class EVMType { Ethereum, Parity };

inline std::string encode_string(const std::string& str) {
  std::string utf8_encoded = encode_str_hex(str);
  std::string l = encode_hex(utf8_encoded.length() / 2);
  std::string ret = left_padding_string(l) + right_padding_string(utf8_encoded);
  return ret;
}

unsigned int get_tip_block_number(const std::string& endpoint);

unsigned int get_txn_block_number(const std::string& endpoint,
                                  const std::string& txn_hash);

std::string get_from_address(const std::string& endpoint);

std::vector<std::string> poll_txs_by_block_hash(const std::string& endpoint,
                                                std::string block_hash);

std::vector<std::string> poll_txs_by_block_number(const std::string& endpoint,
                                                  int block_number);

void unlock_address(const std::string& endpoint, const std::string& address);

///
/// send a deploy transaction and return its receipt
/// @return the receipt hash value to lookup the address of the deployed smart
/// contract
std::string deploy_smart_contract(const std::string& endpoint,
                                  const std::string& from_address,
                                  SmartContractType type);

///
/// try to look up the smart contract address using reciept,
/// if cannot find, crash the program directly
/// @return smart contract address
std::string lookup_smart_contract_address_or_die(const std::string& endpoint,
                                                 const std::string& receipt);

///
/// submit a async do-nothing transaction to the server
/// @return transaction hash returned by server
std::string submit_do_nothing_txn(const std::string& endpoint,
                                  const std::string& from_address,
                                  const std::string& to_address);

///
/// submit a async set transaction to the server
/// @return transaction hash returned by server
std::string submit_set_txn(const std::string& endpoint, const std::string& key,
                           const std::string& val,
                           const std::string& from_address,
                           const std::string& to_address);

///
/// submit a async get transaction to the server
/// @return transaction hash returned by server
std::string submit_get_txn(const std::string& endpoint, const std::string& key,
                           const std::string& from_address,
                           const std::string& to_address);

}  //  EVMUtils
}  //  BBUtils

#endif  // BLOCKBENCH_EVM_UTILS_H_
