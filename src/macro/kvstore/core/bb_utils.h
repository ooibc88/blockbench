#ifndef BLOCKBENCH_BB_UTILS_H_
#define BLOCKBENCH_BB_UTILS_H_

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <restclient-cpp/restclient.h>

namespace BBUtils {

enum class SmartContractType { KVStore, DoNothing, SmallBank };

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

inline std::string encode_hex(unsigned int c) {
  std::ostringstream stm;
  stm << std::hex << c;
  return stm.str();
}

inline unsigned int decode_hex(const std::string &s) {
  unsigned int ret;
  std::stringstream stm;
  stm << std::hex << s;
  stm >> ret;
  return ret;
}

inline std::string encode_str_hex(const std::string &str) {
  std::string result;
  for (unsigned char c : str) result += encode_hex(c);
  return result;
}

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
  return json.substr(quote_sign_pos_2 + 1,
                     quote_sign_pos_3 - quote_sign_pos_2 - 1);
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

/// pad '0' to the right of a string to make it 64-byte alignment
inline std::string right_padding_string(const std::string &str) {
  std::string ret = str;
  for (unsigned i = 0; i < 64 - (str.length() % 64); ++i) ret += '0';
  return ret;
}

/// if len(str) < 64, pad '0' to the left of it to make it 64-byte long
inline std::string left_padding_string(const std::string &str) {
  if (str.length() < 64) {
    std::string ret = str;
    while (ret.length() != 64) ret = "0" + ret;
    return ret;
  } else {
    return str;
  }
}

inline std::string send_jsonrpc_request(const std::string &endpoint,
                                        const std::string &request_header,
                                        const std::string &request_data) {
  return RestClient::post(endpoint, request_header, request_data).body;
}

inline std::string exec(const char* cmd) {
    std::string s(cmd);
    // std::cout << "Execute cmd: " << s << std::endl;
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    // std::cout << "Result: " << result << std::endl;
    return result;
}

}  // BBUtils


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
    item = BBUtils::trim(item);
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
  return BBUtils::trim(result);
}

#endif  // BLOCKBENCH_BB_UTILS_H_
