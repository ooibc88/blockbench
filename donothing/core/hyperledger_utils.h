#ifndef BLOCKBENCH_HYPERLEDGER_UTILS_H_
#define BLOCKBENCH_HYPERLEDGER_UTILS_H_

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>

#include "bb_utils.h"

namespace BBUtils {
namespace HLUtils {

void deploy_chain_code(const std::string &endpoint);

///
/// submit a async do-nothing transaction to the server
/// @return transaction hash returned by server
std::string submit_do_nothing_txn(const std::string& endpoint);

unsigned int get_tip_block_number(const std::string& endpoint);

std::vector<std::string> poll_txs_by_block_number(const std::string& endpoint,
                                                  int block_number);

}  //  HLUtils
}  //  BBUtils

#endif  // BLOCKBENCH_HYPERLEDGER_UTILS_H_
