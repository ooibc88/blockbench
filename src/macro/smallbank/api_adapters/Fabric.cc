#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <sstream>

#include "Fabric.h"
#include "utils/timer.h"

using namespace std;

inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v\"\'") {
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v\"\'")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (right then left)
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v\"\'") {
    return ltrim(rtrim(s, t), t);
}


inline std::vector<std::string> list_field(const std::string &json,
                                              const std::string &key) {
  auto key_pos = json.find(key);
  auto quote_sign_pos_1 = json.find('[', key_pos + 1);
  auto quote_sign_pos_2 = json.find(']', quote_sign_pos_1 + 1);

  std::string s = json.substr(quote_sign_pos_1 + 1,
                          quote_sign_pos_2 - quote_sign_pos_1 - 1);
  std::stringstream ss(s);

  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, ',')) {
    item = trim(item);
    elems.push_back(item);
  }
  return elems;
}
  
inline std::string json_field(const std::string &json,
                                const std::string &key) {
  auto key_pos = json.find(key);
  auto quote_sign_pos_1 = json.find('\'', key_pos + 1);
  auto quote_sign_pos_2 = json.find('\'', quote_sign_pos_1 + 1);
  auto result = json.substr(quote_sign_pos_1 + 1,
                    quote_sign_pos_2 - quote_sign_pos_1 - 1);
  return trim(result);
}


std::string Fabric::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

// get all tx from the start_block until latest
vector<string> Fabric::poll_tx(int block_number) {
  string orderer, peer;
  addresses(&orderer, &peer);
  string cmd = "node api_adapters/fabric-v1.4-node/poll_blk.js " + orderer + " " + peer + " " + std::to_string(block_number);
  string result = this->exec(cmd.c_str());
  if (json_field(result, "status") == "ok") {
    auto r = list_field(result, "txns");
    // std::cout << "Poll Txns: \n[";
    // for (auto i = r.begin(); i != r.end(); ++i) {
    //   std::cout << "\t" << *i << '\n'; 
    // }
    // std::cout << "]" << std::endl;
    return r; 
  } else {
    string msg = json_field(result, "msg");
    cout << "Polling txn fails with err msg " << msg;
    return vector<string>();
  }
}

int Fabric::get_tip_block_number(){
  string orderer, peer;
  addresses(&orderer, &peer);
  string cmd = "node api_adapters/fabric-v1.4-node/query_blk_height.js " + orderer + " " + peer;
  string result = this->exec(cmd.c_str());
  string status = json_field(result, "status");
  // std::cout << "Get tip blk num Result: [" << result << "] status: " << "[" << status << "]" << std::endl;
  if (status == "ok") {
    return stoi(json_field(result, "height"));
  } else {
    string msg = json_field(result, "msg");
    cout << "Get tip block number fails with err msg " << msg;
    return -1;
  }
}

// no error handler, assume always success
void Fabric::deploy(const std::string& path, const std::string& endpoint) {
  std::cout << "Make sure to call deploy.sh manually at the server side." << std::endl;
  sleep(2);  
}

void Fabric::add_to_queue(string json){
  if (json_field(json, "status") != "ok") {
    return;
  }
  double start_time = time_now();
  string txn_hash = json_field(json, "txID"); 
  txlock_->lock(); 
  (*pendingtx_)[txn_hash] = start_time;
  txlock_->unlock();
}

void Fabric::Amalgate(unsigned acc1, unsigned acc2) {
  string orderer, peer;
  addresses(&orderer, &peer);
  string cmd = "node api_adapters/fabric-v1.4-node/invoke.js "  + orderer + " " + peer;
  cmd += " amalgate " + to_string(acc1) + " " + to_string(acc2);
  string result = this->exec(cmd.c_str());
  add_to_queue(result);
}

void Fabric::GetBalance(unsigned acc) {
  string orderer, peer;
  addresses(&orderer, &peer);
  string cmd = "node api_adapters/fabric-v1.4-node/invoke.js "  + orderer + " " + peer;
  cmd += " getBalance " + to_string(acc);
  string result = this->exec(cmd.c_str());
  add_to_queue(result);
}

void Fabric::UpdateBalance(unsigned acc, unsigned amount) {
  string orderer, peer;
  addresses(&orderer, &peer);
  string cmd = "node api_adapters/fabric-v1.4-node/invoke.js "  + orderer + " " + peer;
  cmd += " updateBalance " + to_string(acc) + " " + to_string(amount);
  string result = this->exec(cmd.c_str());
  add_to_queue(result);
}

void Fabric::UpdateSaving(unsigned acc, unsigned amount) {
  string orderer, peer;
  addresses(&orderer, &peer);
  string cmd = "node api_adapters/fabric-v1.4-node/invoke.js "  + orderer + " " + peer;
  cmd += " updateSaving " + to_string(acc) + " " + to_string(amount);
  string result = this->exec(cmd.c_str());
  add_to_queue(result);
}

void Fabric::SendPayment(unsigned acc1, unsigned acc2, unsigned amount) {
  string orderer, peer;
  addresses(&orderer, &peer);
  string cmd = "node api_adapters/fabric-v1.4-node/invoke.js "  + orderer + " " + peer;
  cmd += " sendPayment " + to_string(acc1) + " " + to_string(acc2) + " " + to_string(amount);
  string result = this->exec(cmd.c_str());
  add_to_queue(result);
}

void Fabric::WriteCheck(unsigned acc, unsigned amount) {
  string orderer, peer;
  addresses(&orderer, &peer);
  string cmd = "node api_adapters/fabric-v1.4-node/invoke.js "  + orderer + " " + peer;
  cmd += " writeCheck " + to_string(acc) + " " + to_string(amount);
  string result = this->exec(cmd.c_str());
  add_to_queue(result);
}
