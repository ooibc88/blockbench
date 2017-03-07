#include "hyperledger_utils.h"

namespace BBUtils {
namespace HLUtils {

const std::string CHAIN_END_POINT = "/chain";
const std::string BLOCK_END_POINT = "/chain/blocks";

const std::string REQUEST_HEADERS = "text/json";

const std::string DONOTHING_CHAINCODE_ID =
    "beaacb45774d95081896b61675e73cdb583e71e4e5531c6996015a0e55186373a96eca5b5a"
    "d514c405287754b08cbf4a496e5b5a7ff7531147e9ee8578eae084";

const std::string KVSTORE_CHAINCODE_ID =
    "0b608da26a3960fd8488145c9bd3567a8aae478177968dfd758e1d15f36e782c4a57ae50d7"
    "b8a69b3e98a7cf8e0636113bb944808c650f3d4da485f259a5608c";

const std::string COMMON_COMMAND_1 = "\"}, \"ctorMsg\": {\"function\": \"";
const std::string COMMON_COMMAND_2 = "\",\"args\":[";
const std::string COMMON_COMMAND_SUFFIX = "] } }, \"id\": 1 }";

const std::string INVOKE_DONOTHING_COMMAND_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"invoke\", \
  \"params\": { \
                \"type\": 1, \
                \"chaincodeID\":{ \"name\":\"" +
    DONOTHING_CHAINCODE_ID + COMMON_COMMAND_1;

const std::string INVOKE_KV_COMMAND_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"invoke\", \
  \"params\": { \
                \"type\": 1, \
                \"chaincodeID\":{ \"name\":\"" +
    KVSTORE_CHAINCODE_ID + COMMON_COMMAND_1;

const std::string QUERY_KV_COMMAND_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"query\", \
  \"params\": { \
                \"type\": 1, \
                \"chaincodeID\":{ \"name\":\"" +
    KVSTORE_CHAINCODE_ID + COMMON_COMMAND_1 + "read" + COMMON_COMMAND_2;

const std::string DONOTHING_DEPLOY_COMMAND =
    "{ \
    \"jsonrpc\": \"2.0\", \
    \"method\": \"deploy\", \
    \"params\": { \
                  \"type\": 1, \
                  \"chaincodeID\":{ \"path\": \
                     \"https://github.com/ijingo/chaincode-test/donothing\" }, \
                  \"ctorMsg\": { \"function\":\"init\", \"args\":[] }\
                }, \
    \"id\": 1 \
    }";

const std::string KVSTORE_DEPLOY_COMMAND =
    "{ \
    \"jsonrpc\": \"2.0\", \
    \"method\": \"deploy\", \
    \"params\": { \
                  \"type\": 1, \
                  \"chaincodeID\":{ \"path\": \
                     \"https://github.com/ijingo/chaincode-test/kvstore-v0.6\" }, \
                  \"ctorMsg\": { \"function\":\"init\", \"args\":[] }\
                }, \
    \"id\": 1 \
    }";

std::string compose_read(const std::string &key) {
  return QUERY_KV_COMMAND_PREFIX + "\"" + key + "\"" + COMMON_COMMAND_SUFFIX;
}

std::string compose_write(const std::string &key, const std::string &val) {
  return INVOKE_KV_COMMAND_PREFIX + "write" + COMMON_COMMAND_2 + "\"" + key +
         "\", \"" + val + "\"" + COMMON_COMMAND_SUFFIX;
}

std::string compose_del(const std::string &key) {
  return INVOKE_KV_COMMAND_PREFIX + "del" + COMMON_COMMAND_2 + "\"" + key +
         "\"" + COMMON_COMMAND_SUFFIX;
}

std::string compose_donothing_invoke() {
  return "{\"jsonrpc\": \"2.0\",\"method\": "
         "\"invoke\",\"params\":{\"type\":1,\"chaincodeID\":{\"name\":\"" +
         DONOTHING_CHAINCODE_ID +
         "\"},\"ctorMsg\": {\"function\": \"donothing\", "
         "\"args\":[]}},\"id\":1}";
}

void deploy_chain_code(const std::string &endpoint, SmartContractType type) {
  if (type == SmartContractType::DoNothing)
    send_jsonrpc_request(endpoint, REQUEST_HEADERS, DONOTHING_DEPLOY_COMMAND);
  else
    send_jsonrpc_request(endpoint, REQUEST_HEADERS, KVSTORE_DEPLOY_COMMAND);
}

std::string submit_do_nothing_txn(const std::string &endpoint) {
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS,
                                compose_donothing_invoke());
  return get_json_field(r, "message");
}

std::string submit_get_txn(const std::string &endpoint,
                           const std::string &key) {
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, compose_read(key));
  return get_json_field(r, "message");
}

std::string submit_set_txn(const std::string &endpoint, const std::string &key,
                           const std::string &val) {
  auto r =
      send_jsonrpc_request(endpoint, REQUEST_HEADERS, compose_write(key, val));
  return get_json_field(r, "message");
}

std::string submit_del_txn(const std::string &endpoint,
                           const std::string &key) {
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, compose_del(key));
  return get_json_field(r, "message");
}

int find_tip(std::string json) {
  if (json.find("Failed") != std::string::npos) return -1;
  int key_pos = json.find("height");
  auto close_quote_pos = json.find('\"', key_pos + 1);
  auto comma_pos = json.find(',', key_pos + 1);
  std::string sval =
      json.substr(close_quote_pos + 2, comma_pos - close_quote_pos - 2);
  return stoi(sval);
}

unsigned int get_tip_block_number(const std::string &endpoint) {
  std::string request =
      endpoint.substr(0, endpoint.find("/chaincode")) + CHAIN_END_POINT;
  return find_tip(RestClient::get(request).body);
}

std::vector<std::string> find_tx(std::string json) {
  std::vector<std::string> ret;
  auto key_pos = json.find("txid");
  while (key_pos != std::string::npos) {
    auto quote_sign_pos_1 = json.find('\"', key_pos + 1);
    auto quote_sign_pos_2 = json.find('\"', quote_sign_pos_1 + 1);
    auto quote_sign_pos_3 = json.find('\"', quote_sign_pos_2 + 1);
    ret.push_back(json.substr(quote_sign_pos_2 + 1,
                              quote_sign_pos_3 - quote_sign_pos_2 - 1));
    key_pos = json.find("txid", quote_sign_pos_3 + 1);
  }
  return ret;
}

std::vector<std::string> poll_txs_by_block_number(const std::string &endpoint,
                                                  int block_number) {
  std::string request = endpoint.substr(0, endpoint.find("/chaincode")) +
                        BLOCK_END_POINT + "/" + std::to_string(block_number);
  return find_tx(RestClient::get(request).body);
}

}  // HLUtils
}  // BBUtils
