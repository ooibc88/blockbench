#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <sstream>
#include <restclient-cpp/restclient.h>

#include "FabricV2.h"
#include "utils/timer.h"

// using namespace std;

const std::string HEIGHT_END_POINT = "/height";
const std::string BLOCK_END_POINT = "/block";
const std::string INVOKE_END_POINT = "/invoke";
const std::string QUERY_END_POINT = "/query";

const std::string REQUEST_HEADERS = "application/json";

inline void split(const std::string &s, char delim,
                  std::vector<std::string> *elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    (*elems).push_back(item);
  }
}

inline std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, &elems);
  return elems;
}

inline std::string get_json_field(const std::string &json,
                                  const std::string &key) {
  auto key_pos = json.find(key);
  auto quote_sign_pos_1 = json.find('\"', key_pos + 1);
  auto quote_sign_pos_2 = json.find('\"', quote_sign_pos_1 + 1);
  auto quote_sign_pos_3 = json.find('\"', quote_sign_pos_2 + 1);
  std::string extracted = json.substr(quote_sign_pos_2 + 1, quote_sign_pos_3 - quote_sign_pos_2 - 1);
  return extracted;
}

inline std::vector<std::string> get_list_field(const std::string &json,
                                               const std::string &key) {
  auto key_pos = json.find(key);
  auto quote_sign_pos_1 = json.find('\"', key_pos + 1);
  auto quote_sign_pos_2 = json.find('[', quote_sign_pos_1 + 1);
  auto quote_sign_pos_3 = json.find(']', quote_sign_pos_2 + 1);

  return split(json.substr(quote_sign_pos_2 + 1,
                           quote_sign_pos_3 - quote_sign_pos_2 - 1),
               ',');
}

// get all tx from the start_block until latest
std::vector<std::string> FabricV2::poll_tx(int block_number) {
  std::string serviceAddr;
  this->addresses(&serviceAddr, NULL);
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

int FabricV2::get_tip_block_number() {
  std::string serviceAddr;
  this->addresses(&serviceAddr, NULL);
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

// no error handler, assume always success
void FabricV2::deploy(const std::string& path, const std::string& endpoint) {
  std::cout << "Make sure to deploy Smallbank chaincode based on README.md. " << std::endl;
}

void FabricV2::add_to_queue(std::string json){
  std::string status = get_json_field(json, "status");
  if (status == "0") {
    double start_time = time_now();
    std::string txn_hash = get_json_field(json, "txnID"); 
    txlock_->lock(); 
    (*pendingtx_)[txn_hash] = start_time;
    txlock_->unlock();
  } else {
    std::cerr << "Fail to invoke with error: " 
              << get_json_field(json, "message") << std::endl;
  }
}

std::string FabricV2::randomTxnServiceAddr() {
  std::string blkServiceAddr;
  std::vector<std::string> txnServiceAddrs;
  this->addresses(&blkServiceAddr, &txnServiceAddrs);
  srand (time(NULL));
  size_t i = std::rand() % txnServiceAddrs.size();
  return txnServiceAddrs[i];
}

void FabricV2::InvokeWithOneArg(std::string funcName, std::string arg) {

  const char* COMMAND_TEMPLATE =
        "{ \"function\": \"%s\",  \"args\": [\"%s\"]  }"; 
  char buff[200 + funcName.length() + arg.length()];

  std::sprintf(buff, COMMAND_TEMPLATE, funcName.c_str(), arg.c_str());
  std::string writeReq(buff);
  std::string addr = this->randomTxnServiceAddr();

  auto r = RestClient::post(addr + INVOKE_END_POINT, 
                            REQUEST_HEADERS,
                            writeReq).body;
  this->add_to_queue(r);
}

void FabricV2::InvokeWithTwoArgs(std::string funcName, std::string arg1, std::string arg2) {
  std::string addr = this->randomTxnServiceAddr();
  const char* COMMAND_TEMPLATE =
        "{ \"function\": \"%s\",  \"args\": [\"%s\", \"%s\"]  }"; 
  char buff[200 + funcName.length() + arg1.length() + arg2.length()];

  std::sprintf(buff, COMMAND_TEMPLATE, funcName.c_str(), arg1.c_str(), arg2.c_str());
  std::string writeReq(buff);

  auto r = RestClient::post(addr + INVOKE_END_POINT, 
                            REQUEST_HEADERS,
                            writeReq).body;

  this->add_to_queue(r);
}

void FabricV2::InvokeWithThreeArgs(std::string funcName, std::string arg1, std::string arg2, std::string arg3) {
  std::string addr = this->randomTxnServiceAddr();
  const char* COMMAND_TEMPLATE =
        "{ \"function\": \"%s\",  \"args\": [\"%s\", \"%s\", \"%s\"]  }"; 
  char buff[200 + funcName.length() + arg1.length() + arg2.length() + arg3.length()];

  std::sprintf(buff, COMMAND_TEMPLATE, funcName.c_str(), arg1.c_str(), arg2.c_str(), arg3.c_str());
  std::string writeReq(buff);

  auto r = RestClient::post(addr + INVOKE_END_POINT, 
                            REQUEST_HEADERS,
                            writeReq).body;

  this->add_to_queue(r);
}

void FabricV2::Amalgate(unsigned acc1, unsigned acc2) {
  this->InvokeWithTwoArgs("Almagate", std::to_string(acc1), std::to_string(acc2));
}

void FabricV2::GetBalance(unsigned acc) {
  this->InvokeWithOneArg("GetBalance", std::to_string(acc));
}

void FabricV2::UpdateBalance(unsigned acc, unsigned amount) {
  this->InvokeWithTwoArgs("UpdateBalance", std::to_string(acc), std::to_string(amount));
}

void FabricV2::UpdateSaving(unsigned acc, unsigned amount) {
  this->InvokeWithTwoArgs("UpdateSaving", std::to_string(acc), std::to_string(amount));
}

void FabricV2::SendPayment(unsigned acc1, unsigned acc2, unsigned amount) {
  this->InvokeWithThreeArgs("SendPayment", std::to_string(acc1), std::to_string(acc2), std::to_string(amount));
}

void FabricV2::WriteCheck(unsigned acc, unsigned amount) {
  this->InvokeWithTwoArgs("WriteCheck", std::to_string(acc), std::to_string(amount));
}
