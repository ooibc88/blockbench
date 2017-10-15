#include <restclient-cpp/restclient.h>
#include <string>
#include <vector>
#include <iostream>

#include "SmallBank.h"
#include "utils/chaincode_apis.h"
#include "utils/timer.h"

using namespace RestClient;
using namespace std;
const string CHAIN_END_POINT = "/chain";
const string BLOCK_END_POINT = "/chain/blocks";

string SmallBank::get_json_field(const std::string &json,
                                  const std::string &key) {
  auto key_pos = json.find(key);
  auto quote_sign_pos_1 = json.find('\"', key_pos + 1);
  auto quote_sign_pos_2 = json.find('\"', quote_sign_pos_1 + 1);
  auto quote_sign_pos_3 = json.find('\"', quote_sign_pos_2 + 1);
  return json.substr(quote_sign_pos_2 + 1,
                     quote_sign_pos_3 - quote_sign_pos_2 - 1);
}

vector<string> SmallBank::find_tx(string json){
  vector<string> ss;
  unsigned int key_pos = json.find("txid");
  while (key_pos!=string::npos){
    auto quote_sign_pos_1 = json.find('\"', key_pos + 1);
    auto quote_sign_pos_2 = json.find('\"', quote_sign_pos_1 + 1);
    auto quote_sign_pos_3 = json.find('\"', quote_sign_pos_2 + 1);
    ss.push_back(json.substr(quote_sign_pos_2 + 1,
          quote_sign_pos_3 - quote_sign_pos_2 - 1));
    key_pos = json.find("txid", quote_sign_pos_3+1);
  }
  return ss;
}

int SmallBank::find_tip(string json){
  if (json.find("Failed")!=string::npos)
    return -1;
  int key_pos = json.find("height");
  auto close_quote_pos = json.find('\"',key_pos+1);
  auto comma_pos = json.find(',', key_pos+1);
  string sval = json.substr(close_quote_pos+2, comma_pos-close_quote_pos-2);
  return stoi(sval);
}

// get all tx from the start_block until latest
vector<string> SmallBank::poll_tx(int block_number) {
  string request = endpoint_.substr(0,endpoint_.find("/chaincode"))+BLOCK_END_POINT+"/"+std::to_string(block_number);
  return find_tx(get(request).body);
}

unsigned int SmallBank::get_tip_block_number(){
  string request = endpoint_.substr(0,endpoint_.find("/chaincode"))+CHAIN_END_POINT;
  return find_tip(get(request).body);
}

void SmallBank::add_to_queue(string json){
  double start_time = time_now();
  string txn_hash = get_json_field(json, "message"); 
  txlock_->lock(); 
  (*pendingtx_)[txn_hash] = start_time;
  txlock_->unlock();
}

// no error handler, assume always success
void SmallBank::deploy(const std::string& path, const std::string& endpoint) {
  endpoint_ = endpoint; 
  std::vector<std::string> args;
  cout << "deploy request " << compose_deploy(path, args) << " to " << endpoint_ << endl; 
  Response r = post(endpoint_, REQUEST_HEADERS,
  compose_deploy("https://github.com/ijingo/chaincode-test/smallbank", args));
  chaincode_name_ = get_json_field(r.body, "message");
  cout << "Deployed new chaincode: " << chaincode_name_ << endl;
  sleep(2);  
}

void SmallBank::Amalgate(unsigned acc1, unsigned acc2) {
  vector<string> args;
  args.push_back(to_string(acc1));
  args.push_back(to_string(acc2));
  Response r = post(endpoint_, REQUEST_HEADERS,
       compose_invoke(chaincode_name_, "amalgate", args));
  add_to_queue(r.body); 
}

void SmallBank::GetBalance(unsigned acc) {
  vector<string> args;
  args.push_back(to_string(acc));
  Response r = post(endpoint_, REQUEST_HEADERS,
       compose_invoke(chaincode_name_, "getBalance", args));
  add_to_queue(r.body); 
  // cout << compose_invoke(chaincode_name_, "getBalance", args) << endl;
}

void SmallBank::UpdateBalance(unsigned acc, unsigned amount) {
  vector<string> args;
  args.push_back(to_string(acc));
  args.push_back(to_string(amount));
  Response r = post(endpoint_, REQUEST_HEADERS,
       compose_invoke(chaincode_name_, "updateBalance", args));
  add_to_queue(r.body); 
  // cout << compose_invoke(chaincode_name_, "updateBalance", args) << endl;
}

void SmallBank::UpdateSaving(unsigned acc, unsigned amount) {
  vector<string> args;
  args.push_back(to_string(acc));
  args.push_back(to_string(amount));
  Response r = post(endpoint_, REQUEST_HEADERS,
       compose_invoke(chaincode_name_, "updateSaving", args));
  add_to_queue(r.body); 
  // cout << compose_invoke(chaincode_name_, "updateSaving", args) << endl;
}

void SmallBank::SendPayment(unsigned acc1, unsigned acc2, unsigned amount) {
  vector<string> args;
  args.push_back(to_string(acc1));
  args.push_back(to_string(acc2));
  args.push_back(to_string(amount));
  Response r = post(endpoint_, REQUEST_HEADERS,
       compose_invoke(chaincode_name_, "sendPayment", args));
  add_to_queue(r.body); 
  // cout << compose_invoke(chaincode_name_, "sendPayment", args) << endl;
}

void SmallBank::WriteCheck(unsigned acc, unsigned amount) {
  vector<string> args;
  args.push_back(to_string(acc));
  args.push_back(to_string(amount));
  Response r = post(endpoint_, REQUEST_HEADERS,
       compose_invoke(chaincode_name_, "writeCheck", args));
  add_to_queue(r.body); 
  // cout << compose_invoke(chaincode_name_, "sendPayment", args) << endl;
}
