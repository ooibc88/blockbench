#include "fabricv2_utils.h"

namespace BBUtils {
namespace FABV2Utils {

const std::string HEIGHT_END_POINT = "/height";
const std::string BLOCK_END_POINT = "/block";
const std::string INVOKE_END_POINT = "/invoke";
const std::string QUERY_END_POINT = "/query";

const std::string REQUEST_HEADERS = "application/json";

const std::string INVOKE_DONOTHING_COMMAND =
    "{\
  \"function\": \"Invoke\", \
  \"args\": [] \
  }"; 
  
 const char* DEL_KV_COMMAND_TEMPLATE =
    "{\
  \"function\": \"Del\", \
  \"args\": [\"%s\"] \
  }"; 

 const char* WRITE_KV_COMMAND_TEMPLATE =
    "{\
  \"function\": \"Write\", \
  \"args\": [\"%s\", \"%s\"] \
  }"; 

std::string compose_write(const std::string &key, const std::string &val) {
  char buff[200 + key.length() + val.length()];
  std::sprintf(buff, WRITE_KV_COMMAND_TEMPLATE, key.c_str(), val.c_str());
  std::string writeReq(buff);
  return writeReq;
}

std::string compose_del(const std::string &key) {
  char buff[200 + key.length()];
  std::sprintf(buff, DEL_KV_COMMAND_TEMPLATE, key.c_str());
  return std::string(buff);
}

std::string compose_donothing_invoke() {
  return INVOKE_DONOTHING_COMMAND;
}

void deploy_chain_code(const std::string &endpoint, SmartContractType type) {
  if (type == SmartContractType::DoNothing) {
    std::cout << "Make sure to deploy DoNothing chaincode based on README.md. " << std::endl;
  } else {
    std::cout << "Make sure to deploy YCSB chaincode based on README.md. " << std::endl;
  }
}

std::string submit_do_nothing_txn(const std::string &serviceAddr) {
  auto r = send_jsonrpc_request(serviceAddr + INVOKE_END_POINT, 
                                REQUEST_HEADERS,
                                compose_donothing_invoke());
  if (get_json_field(r, "status") == "1") {
    std::cerr << "Fail to invoke with error " 
              << get_json_field(r, "message") << std::endl;
    return "";
  } else {
    return get_json_field(r, "txnID");
  }
}

std::string submit_get_txn(const std::string &serviceAddr,
                           const std::string &key) {
  char buff[100 + key.length()];
  std::sprintf(buff, "?function=%s", key.c_str());
  std::string requestArg(buff);
  auto r = RestClient::get(serviceAddr + QUERY_END_POINT + requestArg).body;
  if (get_json_field(r, "status") == "1") {
    std::cerr << "Fail to read with error " 
              << get_json_field(r, "message") << std::endl;
    return "";
  } else {
    return ""; // no txnID will return
  }
}

std::string submit_set_txn(const std::string &serviceAddr, const std::string &key, const std::string &val) {
  std::string writeReq = compose_write(key, val);
  auto r = RestClient::post(serviceAddr + INVOKE_END_POINT, 
                                REQUEST_HEADERS,
                                writeReq).body;
  if (get_json_field(r, "status") == "1") {
    std::cerr << "Fail to write with error: " 
              << get_json_field(r, "message") << std::endl;
  } else {
    std::string txnID = get_json_field(r, "txnID");
    // std::cout << "TxnID: [" << txnID << "]" << std::endl;
    return txnID;
  }
}

std::string submit_del_txn(const std::string &serviceAddr,
                           const std::string &key) {
  auto r = RestClient::post(serviceAddr + INVOKE_END_POINT, 
                                REQUEST_HEADERS,
                                compose_del(key)).body;
  if (get_json_field(r, "status") == "1") {
    std::cerr << "Fail to remove with error " 
              << get_json_field(r, "message") << std::endl;
    return "";
  } else {
    return get_json_field(r, "txnID");
  }
}

int get_tip_block_number(const std::string &serviceAddr) {
  auto r = RestClient::get(serviceAddr + HEIGHT_END_POINT).body;
  if (get_json_field(r, "status") == "1") {
    std::cerr << "Fail to query for the ledger height " 
              << get_json_field(r, "message") << std::endl;
    return -1;
  } else {
    std::string height = get_json_field(r, "height");
    return atoi(height.c_str())-1;
  }
}

std::vector<std::string> poll_txs_by_block_number(const std::string &serviceAddr,
                                                  int block_number) {
  char buff[100];
  std::sprintf(buff, "?num=%d", block_number);
  std::string requestArg(buff);
  auto r = RestClient::get(serviceAddr + BLOCK_END_POINT + requestArg).body;
  if (get_json_field(r, "status") == "1") {
    std::cerr << "Fail to read with error " 
              << get_json_field(r, "message") << std::endl;
    return std::vector<std::string>();
  } else {
    std::vector<std::string> txns = get_list_field(r, "txns");
    std::vector<std::string> trimedTxns;
    // std::cout << "Block " << block_number << " has txns: " << std::endl;
    for (auto i = txns.begin(); i != txns.end(); ++i) {
      // remove the surrounding "" and the last whitespace
      std::string trimed = (*i).substr(1, i->length()-2); 
      trimedTxns.push_back(trimed);
      // std::cout << "\t[" << trimed << "]" << std::endl;
    }
    return trimedTxns;
  }
}

}  // FABV2Utils
}  // BBUtils
