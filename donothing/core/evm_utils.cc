#include "evm_utils.h"
#include <iostream>

namespace BBUtils {
namespace EVMUtils {

const std::string REQUEST_HEADERS = "application/json";
const std::string UNLOCK_ACCOUNT_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"personal_unlockAccount\", \
  \"params\": [\"";

const std::string UNLOCK_ACCOUNT_SUFFIX =
    "\",\"\",null], \
      \"id\": 1}";

const std::string SEND_TXN_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"eth_sendTransaction\", \
  \"params\": [{ \"gas\": \"0x100000\", \
                \"gasPrice\": \"0x0\", \
                \"from\": \"";

const std::string CALL_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"eth_call\", \
  \"params\": [{ \"gas\": \"0x100000\",\
                \"gasPrice\": \"0x0\", \
                \"from\": \"";

const std::string GET_TXN_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"eth_getTransactionByHash\", \
  \"params\": [\"";

const std::string MIDDLE_PART_1 = "\", \"to\": \"";
const std::string MIDDLE_PART_2 = "\", \"data\": \"";
const std::string SEND_TXN_SUFFIX = "\"}],\"id\":1}";
const std::string CALL_SUFFIX = "\"},\"latest\"],\"id\":1}";
const std::string GET_TXN_SUFFIX = "\"],\"id\":1}";

const std::string GET_BLOCK_BY_NUMBER_PREFIX =
    "{\"jsonrpc\":\"2.0\", \
      \"method\":\"eth_getBlockByNumber\", \
      \"params\":[\"";

const std::string GET_BLOCK_BY_NUMBER_SUFFIX =
    "\", false], \
      \"id\": 1}";

const std::string GET_BLOCK_BY_HASH_PREFIX =
    "{\"jsonrpc\":\"2.0\", \
      \"method\":\"eth_getBlockByHash\", \
      \"params\":[\"";

const std::string GET_BLOCK_BY_HASH_SUFFIX =
    "\", false], \
      \"id\": 1}";

const std::string GET_BLOCKNUMBER =
    "{\"jsonrpc\":\"2.0\", \
      \"method\":\"eth_blockNumber\", \
      \"params\":[], \
      \"id\": 1}";

const std::string GET_ACCOUNTS =
    "{\"jsonrpc\":\"2.0\",\"method\":\"eth_accounts\",\"params\":[],\"id\":1}";

const std::string DEPLOY_SMARTCONTRACT_PREFIX =
    " {\"jsonrpc\":\"2.0\",\
  \"method\":\"eth_sendTransaction\",\
  \"params\": [{ \"gas\": \"0x10000\",\
                \"gasPrice\": \"0x0\", \
               \"from\": \"";

const std::string DEPLOY_DONOTHING_SMARTCONTRACT_SUFFIX =
    "\", \"data\": "
    "\"0x60606040523415600b57fe5b5b60788061001a6000396000f30060606040526000357c"
    "0100000000000000000000000000000000000000000000000000000000900463ffffffff16"
    "8063448f30a314603a575bfe5b3415604157fe5b60476049565b005b5b5600a165627a7a72"
    "30582006401d873570c51618d5e2963a0048502e8f49f465e30cf2624369a1c7db3fa00029"
    "\"}],\"id\":1}";

const std::string GET_SMART_CONTRACT_ADDRESS_PREFIX =
    "{\"jsonrpc\":\"2.0\",\
       \"method\":\"eth_getTransactionReceipt\",\
       \"params\":[\"";

const std::string GET_SMART_CONTRACT_ADDRESS_SUFFIX = "\"],\"id\":1}";

const std::string DONOTHING_METHOD_SIG = "0x448f30a3";
const std::string SET_METHOD_SIG =
    "0xe942b51600000000000000000000000000000000000000000000000000000000000000"
    "40";
const std::string GET_METHOD_SIG =
    "0x693ec85e00000000000000000000000000000000000000000000000000000000000000"
    "20";

std::string encode_set(const std::string &key, const std::string &value) {
  std::string ret = SET_METHOD_SIG;
  std::string argument_1 = encode_string(key);
  ret += left_padding_string(encode_hex(argument_1.length()));
  ret += argument_1 + encode_string(value);
  return ret;
}

std::string encode_get(const std::string &key) {
  return GET_METHOD_SIG + encode_string(key);
}

std::string compose_donothing_tx_data(const std::string &from_address,
                                      const std::string &to_address) {
  return SEND_TXN_PREFIX + from_address + MIDDLE_PART_1 + to_address +
         MIDDLE_PART_2 + DONOTHING_METHOD_SIG + SEND_TXN_SUFFIX;
}

std::string compose_read(const std::string &key,
                         const std::string &from_address,
                         const std::string &to_address) {
  return CALL_PREFIX + from_address + MIDDLE_PART_1 + to_address +
         MIDDLE_PART_2 + encode_get(key) + CALL_SUFFIX;
}

std::string compose_write(const std::string &key, const std::string &val,
                          const std::string &from_address,
                          const std::string &to_address) {
  return SEND_TXN_PREFIX + from_address + MIDDLE_PART_1 + to_address +
         MIDDLE_PART_2 + encode_set(key, val) + SEND_TXN_SUFFIX;
}

std::string compose_get_transaction(const std::string &txn_hash) {
  return GET_TXN_PREFIX + txn_hash + GET_TXN_SUFFIX;
}

unsigned int get_tip_block_number(const std::string &endpoint) {
  return decode_hex(get_json_field(
      send_jsonrpc_request(endpoint, REQUEST_HEADERS, GET_BLOCKNUMBER),
      "result"));
}

unsigned int get_txn_block_number(const std::string &endpoint,
                                  const std::string &txn_hash) {
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS,
                                compose_get_transaction(txn_hash));
  // in case this transaction haven't been mined, return MAXIMUM uint.
  if (r.find("\"blockNumber\":null") != std::string::npos) return -1;
  return decode_hex(get_json_field(r, "blockNumber"));
}

std::string get_from_address(const std::string &endpoint) {
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, GET_ACCOUNTS);
  return get_json_field(r, "result");
}

std::vector<std::string> poll_txs_by_block_hash(const std::string &endpoint,
                                                std::string block_hash) {
  std::string request =
      GET_BLOCK_BY_HASH_PREFIX + block_hash + GET_BLOCK_BY_HASH_SUFFIX;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, request);
  auto ret = get_list_field(r, "transactions");
  return ret;
}

std::vector<std::string> poll_txs_by_block_number(const std::string &endpoint,
                                                  int block_number) {
  std::string request = GET_BLOCK_BY_NUMBER_PREFIX + std::to_string(block_number) +
                   GET_BLOCK_BY_NUMBER_SUFFIX;

  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, request);

  std::vector<std::string> ret = get_list_field(r, "transactions");
  std::vector<std::string> uncles = get_list_field(r, "uncles");
  for (std::string uncle : uncles) {
    std::vector<std::string> uncletxs = poll_txs_by_block_hash(endpoint, uncle);
    for (std::string tx : uncletxs) ret.push_back(tx);
  }
  return ret;
}

void unlock_address(const std::string &endpoint,
                           const std::string &address) {
  send_jsonrpc_request(endpoint, REQUEST_HEADERS,
                       UNLOCK_ACCOUNT_PREFIX + address + UNLOCK_ACCOUNT_SUFFIX);
}

std::string deploy_smart_contract(const std::string &endpoint,
                                  const std::string &from_address,
                                  SmartContractType type) {
  std::string txn_data = DEPLOY_SMARTCONTRACT_PREFIX + from_address +
                         DEPLOY_DONOTHING_SMARTCONTRACT_SUFFIX;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, txn_data);
  return get_json_field(r, "result");
}

std::string lookup_smart_contract_address_or_die(const std::string &endpoint,
                                                 const std::string &receipt) {
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS,
                                GET_SMART_CONTRACT_ADDRESS_PREFIX + receipt +
                                    GET_SMART_CONTRACT_ADDRESS_SUFFIX);

  assert(r.find("\"result\":null") == std::string::npos);
  return get_json_field(r, "contractAddress");
}

std::string submit_do_nothing_txn(const std::string &endpoint,
                                  const std::string &from_address,
                                  const std::string &to_address) {
  auto r = send_jsonrpc_request(
      endpoint, REQUEST_HEADERS,
      compose_donothing_tx_data(from_address, to_address));
  return get_json_field(r, "result");
}

}  // EVMUtils
}  // BBUtils
