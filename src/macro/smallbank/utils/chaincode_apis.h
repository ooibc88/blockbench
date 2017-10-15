#ifndef SMARTCONTRACT_DRIVER_UTILS_CHAINCODE_APIS_H_
#define SMARTCONTRACT_DRIVER_UTILS_CHAINCODE_APIS_H_

#include <string>
#include <vector>
#include <sstream>
#include <cassert>

//const std::string REQUEST_HEADERS = "text/json";
const std::string REQUEST_HEADERS = "application/json";

const std::string INVOKE_COMMAND_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"invoke\", \
  \"params\": { \
                \"type\": 1, \
                \"chaincodeID\":{ \"name\":\"";

const std::string QUERY_COMMAND_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"query\", \
  \"params\": { \
                \"type\": 1, \
                \"chaincodeID\":{ \"name\":\"";

const std::string COMMON_COMMAND_1 = "\"}, \"ctorMsg\": {\"function\": \"";
const std::string COMMON_COMMAND_2 = "\",\"args\":[";

const std::string DEPLOY_COMMAND_PREFIX =
    "{ \
    \"jsonrpc\": \"2.0\", \
    \"method\": \"deploy\", \
    \"params\": { \
                  \"type\": 1, \
                  \"chaincodeID\":{ \"path\": \"";

const std::string COMMON_COMMAND_SUFFIX = "] } }, \"id\": 1 }";

inline std::string compose_invoke(const std::string& chaincodeID,
                                  const std::string& func,
                                  const std::vector<std::string>& args) {
  std::string str = "";
  if (args.size() >= 1) str += args[0];
  for (unsigned i = 1; i < args.size(); ++i) {
    str += "\",\"" + args[i];
  }
  if (args.size() != 0) str = "\"" + str + "\"";
  return INVOKE_COMMAND_PREFIX + chaincodeID + COMMON_COMMAND_1 + func +
         COMMON_COMMAND_2 + str + COMMON_COMMAND_SUFFIX;
}

inline std::string compose_query(const std::string& chaincodeID,
                                 const std::string& func,
                                 const std::vector<std::string>& args) {
  std::string str = "";
  if (args.size() >= 1) str += args[0];
  for (unsigned i = 1; i < args.size(); ++i) {
    str += "\",\"" + args[i];
  }
  if (args.size() != 0) str = "\"" + str + "\"";
  return QUERY_COMMAND_PREFIX + chaincodeID + COMMON_COMMAND_1 + func +
         COMMON_COMMAND_2 + str + COMMON_COMMAND_SUFFIX;
}

inline std::string compose_deploy(const std::string& path,
                                  const std::vector<std::string>& args) {
  std::string str = "";
  if (args.size() >= 1) str += args[0];
  for (unsigned i = 1; i < args.size(); ++i) {
    str += "\",\"" + args[i];
  }
  if (args.size() != 0) str = "\"" + str + "\"";
  return DEPLOY_COMMAND_PREFIX + path + COMMON_COMMAND_1 + "init" +
         COMMON_COMMAND_2 + str + COMMON_COMMAND_SUFFIX;
}

//-----------
const std::string DEPLOY_SMARTCONTRACT_PREFIX =
    " {\"jsonrpc\":\"2.0\",\
  \"method\":\"eth_sendTransaction\",\
  \"params\": [{ \"gas\": \"0x1000000\",\
                \"gasPrice\": \"0x0\", \
               \"from\": \"";

const std::string DEPLOY_SB_SMARTCONTRACT_SUFFIX =
    "\", \"data\": "
    "\"0x6060604052341561000f57600080fd5b5b610c4e8061001f6000396000f30060606040523615610076576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff1680630b488b371461007b5780630be8374d146100e15780633a51d24614610147578063870187eb146101a4578063901d706f1461020a578063ca305435146102aa575b600080fd5b341561008657600080fd5b6100df600480803590602001908201803590602001908080601f01602080910402602001604051908101604052809392919081815260200183838082843782019150505050505091908035906020019091905050610353565b005b34156100ec57600080fd5b610145600480803590602001908201803590602001908080601f01602080910402602001604051908101604052809392919081815260200183838082843782019150505050505091908035906020019091905050610442565b005b341561015257600080fd5b6101a2600480803590602001908201803590602001908080601f01602080910402602001604051908101604052809392919081815260200183838082843782019150505050505091905050610629565b005b34156101af57600080fd5b610208600480803590602001908201803590602001908080601f0160208091040260200160405190810160405280939291908181526020018383808284378201915050505050509190803590602001909190505061074e565b005b341561021557600080fd5b6102a8600480803590602001908201803590602001908080601f0160208091040260200160405190810160405280939291908181526020018383808284378201915050505050509190803590602001908201803590602001908080601f0160208091040260200160405190810160405280939291908181526020018383808284378201915050505050509190505061087a565b005b34156102b557600080fd5b610351600480803590602001908201803590602001908080601f0160208091040260200160405190810160405280939291908181526020018383808284378201915050505050509190803590602001908201803590602001908080601f01602080910402602001604051908101604052809392919081815260200183838082843782019150505050505091908035906020019091905050610a47565b005b6000806000846040518082805190602001908083835b60208310151561038f57805182525b602082019150602081019050602083039250610369565b6001836020036101000a03801982511681845116808217855250505050505090500191505090815260200160405180910390205491508290508082016000856040518082805190602001908083835b60208310151561040457805182525b6020820191506020810190506020830392506103de565b6001836020036101000a0380198251168184511680821785525050505050509050019150509081526020016040518091039020819055505b50505050565b60008060006001856040518082805190602001908083835b60208310151561048057805182525b60208201915060208101905060208303925061045a565b6001836020036101000a03801982511681845116808217855250505050505090500191505090815260200160405180910390205492506000856040518082805190602001908083835b6020831015156104ef57805182525b6020820191506020810190506020830392506104c9565b6001836020036101000a03801982511681845116808217855250505050505090500191505090815260200160405180910390205491508390508183018110156105ad576001818403036001866040518082805190602001908083835b60208310151561057157805182525b60208201915060208101905060208303925061054b565b6001836020036101000a038019825116818451168082178552505050505050905001915050908152602001604051809103902081905550610621565b8083036001866040518082805190602001908083835b6020831015156105e957805182525b6020820191506020810190506020830392506105c3565b6001836020036101000a0380198251168184511680821785525050505050509050019150509081526020016040518091039020819055505b5b5050505050565b600080600080846040518082805190602001908083835b60208310151561066657805182525b602082019150602081019050602083039250610640565b6001836020036101000a03801982511681845116808217855250505050505090500191505090815260200160405180910390205492506001846040518082805190602001908083835b6020831015156106d557805182525b6020820191506020810190506020830392506106af565b6001836020036101000a038019825116818451168082178552505050505050905001915050908152602001604051809103902054915081830190507ff0b60c507a2e93235b6bed490cd5616aa2f43a849c3d167e96d6c5099b5524e5816040518082815260200191505060405180910390a15b50505050565b60008060006001856040518082805190602001908083835b60208310151561078c57805182525b602082019150602081019050602083039250610766565b6001836020036101000a03801982511681845116808217855250505050505090500191505090815260200160405180910390205492508391508183019050806001866040518082805190602001908083835b60208310151561080457805182525b6020820191506020810190506020830392506107de565b6001836020036101000a0380198251168184511680821785525050505050509050019150509081526020016040518091039020819055507ff0b60c507a2e93235b6bed490cd5616aa2f43a849c3d167e96d6c5099b5524e5816040518082815260200191505060405180910390a15b5050505050565b6000806000846040518082805190602001908083835b6020831015156108b657805182525b602082019150602081019050602083039250610890565b6001836020036101000a03801982511681845116808217855250505050505090500191505090815260200160405180910390205491506001836040518082805190602001908083835b60208310151561092557805182525b6020820191506020810190506020830392506108ff565b6001836020036101000a038019825116818451168082178552505050505050905001915050908152602001604051809103902054905060006001856040518082805190602001908083835b60208310151561099657805182525b602082019150602081019050602083039250610970565b6001836020036101000a0380198251168184511680821785525050505050509050019150509081526020016040518091039020819055508082016000846040518082805190602001908083835b602083101515610a0957805182525b6020820191506020810190506020830392506109e3565b6001836020036101000a0380198251168184511680821785525050505050509050019150509081526020016040518091039020819055505b50505050565b60008060006001866040518082805190602001908083835b602083101515610a8557805182525b602082019150602081019050602083039250610a5f565b6001836020036101000a03801982511681845116808217855250505050505090500191505090815260200160405180910390205492506001856040518082805190602001908083835b602083101515610af457805182525b602082019150602081019050602083039250610ace565b6001836020036101000a038019825116818451168082178552505050505050905001915050908152602001604051809103902054915083905080830392508082019150826001876040518082805190602001908083835b602083101515610b7157805182525b602082019150602081019050602083039250610b4b565b6001836020036101000a038019825116818451168082178552505050505050905001915050908152602001604051809103902081905550816001866040518082805190602001908083835b602083101515610be257805182525b602082019150602081019050602083039250610bbc565b6001836020036101000a0380198251168184511680821785525050505050509050019150509081526020016040518091039020819055505b5050505050505600a165627a7a7230582089fc7378aff690f59b99745baed114eb59b2146a5b4ceea6720fbd3007a6206a0029\"}],\"id\":1}";

const std::string GET_SMART_CONTRACT_ADDRESS_PREFIX =
    "{\"jsonrpc\":\"2.0\",\
       \"method\":\"eth_getTransactionReceipt\",\
       \"params\":[\"";

const std::string GET_SMART_CONTRACT_ADDRESS_SUFFIX = "\"],\"id\":1}";

const std::string GET_ACCOUNTS =
    "{\"jsonrpc\":\"2.0\",\"method\":\"eth_accounts\",\"params\":[],\"id\":1}";

const std::string GET_BLOCKNUMBER =
    "{\"jsonrpc\":\"2.0\", \
      \"method\":\"eth_blockNumber\", \
      \"params\":[], \
      \"id\": 1}";

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
  \"params\": [{ \"gas\": \"0x1000000\", \
                \"gasPrice\": \"0x0\", \
                \"from\": \"";

const std::string CALL_PREFIX =
    "{\
  \"jsonrpc\": \"2.0\", \
  \"method\": \"eth_call\", \
  \"params\": [{ \"gas\": \"0x1000000\",\
                \"gasPrice\": \"0x0\", \
                \"from\": \"";

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

const std::string MIDDLE_PART_1 = "\", \"to\": \"";
const std::string MIDDLE_PART_2 = "\", \"data\": \"";                
const std::string SEND_TXN_SUFFIX = "\"}],\"id\":1}";
const std::string CALL_SUFFIX = "\"},\"latest\"],\"id\":1}";

const std::string AMALGATE_METHOD_SIG =
    "0x901d706f00000000000000000000000000000000000000000000000000000000000000"
    "40";

const std::string GETBALANCE_METHOD_SIG =
    "0x3a51d24600000000000000000000000000000000000000000000000000000000000000"
    "20";

const std::string UPDATEBALANCE_METHOD_SIG =
    "0x870187eb00000000000000000000000000000000000000000000000000000000000000"
    "40";

const std::string UPDATESAVING_METHOD_SIG =
    "0x0b488b3700000000000000000000000000000000000000000000000000000000000000"
    "40";

const std::string WRITECHECK_METHOD_SIG =
    "0x0be8374d00000000000000000000000000000000000000000000000000000000000000"
    "40";

const std::string SENDPAYMENT_METHOD_SIG =
    "0xca30543500000000000000000000000000000000000000000000000000000000000000"
    "60";

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

inline unsigned int decode_hex(const std::string &s) {
  unsigned int ret;
  std::stringstream stm;
  stm << std::hex << s;
  stm >> ret;
  return ret;
}

inline std::string encode_hex(unsigned int c) {
  std::ostringstream stm;
  stm << std::hex << c;
  return stm.str();
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

inline vector<string> find_tx(string json){
  vector<string> ss;
  uint key_pos = json.find("txid");
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

inline int find_tip(string json){
  if (json.find("Failed")!=string::npos)
    return -1;
  int key_pos = json.find("height");
  auto close_quote_pos = json.find('\"',key_pos+1);
  auto comma_pos = json.find(',', key_pos+1);
  string sval = json.substr(close_quote_pos+2, comma_pos-close_quote_pos-2);
  return stoi(sval);
}

inline std::string send_jsonrpc_request(const std::string &endpoint,
                                        const std::string &request_header,
                                        const std::string &request_data) {
  return RestClient::post(endpoint, request_header, request_data).body;
}

inline std::string deploy_smart_contract(const std::string &endpoint,
                                  const std::string &from_address,
                                  const string type) {
  std::string txn_data = DEPLOY_SMARTCONTRACT_PREFIX + from_address;
  txn_data += DEPLOY_SB_SMARTCONTRACT_SUFFIX;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, txn_data);
  return get_json_field(r, "result");
}

inline std::string get_from_address(const std::string &endpoint) {
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, GET_ACCOUNTS);
  return get_json_field(r, "result");
}

inline std::string lookup_smart_contract_address_or_die(const std::string &endpoint,
                                                 const std::string &receipt) {
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS,
                                GET_SMART_CONTRACT_ADDRESS_PREFIX + receipt +
                                    GET_SMART_CONTRACT_ADDRESS_SUFFIX);
  //std::cout << "smart contract address r:" << r << " receipt:"<< receipt << std::endl;
  //assert(r.find("\"result\":null") == std::string::npos);
  return get_json_field(r, "contractAddress");
}

inline void unlock_address(const std::string &endpoint, const std::string &address) {
  send_jsonrpc_request(endpoint, REQUEST_HEADERS,
                       UNLOCK_ACCOUNT_PREFIX + address + UNLOCK_ACCOUNT_SUFFIX);
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

inline std::string encode_str_hex(const std::string &str) {
  std::string result;
  for (unsigned char c : str) result += encode_hex(c);
  return result;
}

inline std::string encode_string(const std::string& str) {
  std::string utf8_encoded = encode_str_hex(str);
  std::string l = encode_hex(utf8_encoded.length() / 2);
  std::string ret = left_padding_string(l) + right_padding_string(utf8_encoded);
  return ret;
}

inline std::string encode_amalgate(const std::string &arg0, const std::string &arg1) {
  std::string ret = AMALGATE_METHOD_SIG;
  std::string argument_1 = encode_string(arg0);
  ret += left_padding_string(encode_hex(argument_1.length()));
  ret += argument_1 + encode_string(arg1); 
  return ret;
}

inline std::string encode_getBalance(const std::string &arg) {
  return GETBALANCE_METHOD_SIG + encode_string(arg);
}

inline std::string encode_updateBalance(const std::string &arg, const unsigned &amount) {
  return UPDATEBALANCE_METHOD_SIG + left_padding_string(encode_hex(amount))
      + encode_string(arg);
}

inline std::string encode_updateSaving(const std::string &arg, const unsigned &amount) {
  return UPDATESAVING_METHOD_SIG + left_padding_string(encode_hex(amount))
      + encode_string(arg);
}

inline std::string encode_writeCheck(const std::string &arg, const unsigned &amount) {
  return WRITECHECK_METHOD_SIG + left_padding_string(encode_hex(amount))
      + encode_string(arg);
}

inline std::string encode_sendPayment(const std::string &arg0, const std::string &arg1,
                                 const unsigned &amount) {
  std::string ret = SENDPAYMENT_METHOD_SIG;
  std::string argument_1 = encode_string(arg0);
  ret += left_padding_string(encode_hex(argument_1.length())) 
        + left_padding_string(encode_hex(amount));
  ret += argument_1 + encode_string(arg1); 
  return ret;
}

inline std::string submit_amalgate_txn(const std::string &arg0, const std::string &arg1,
                           const std::string &endpoint, const std::string &from_address,
                           const std::string &to_address) {
  
  std::string request_json = SEND_TXN_PREFIX + from_address + MIDDLE_PART_1 + to_address +
         MIDDLE_PART_2 + encode_amalgate(arg0, arg1) + SEND_TXN_SUFFIX;
  //std::cout << "request_json:" << request_json << std::endl;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, request_json);
  //std::cout << "result:" << r << std::endl;
  return get_json_field(r, "result");
}

inline std::string submit_getBalance_txn(const std::string &arg,
                           const std::string &endpoint, const std::string &from_address,
                           const std::string &to_address) {
  
  std::string request_json = SEND_TXN_PREFIX + from_address + MIDDLE_PART_1 + to_address +
         MIDDLE_PART_2 + encode_getBalance(arg) + SEND_TXN_SUFFIX;
  //std::cout << "request_json:" << request_json << std::endl;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, request_json);
  //std::cout << "result:" << r << std::endl;
  return get_json_field(r, "result");
}

inline std::string submit_updateBalance_txn(const std::string &arg, const unsigned &amount,
                           const std::string &endpoint, const std::string &from_address,
                           const std::string &to_address) {
  
  std::string request_json = SEND_TXN_PREFIX + from_address + MIDDLE_PART_1 + to_address +
         MIDDLE_PART_2 + encode_updateBalance(arg, amount) + SEND_TXN_SUFFIX;
  //std::cout << "request_json:" << request_json << std::endl;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, request_json);
  //std::cout << "result:" << r << std::endl;
  return get_json_field(r, "result");
}

inline std::string submit_updateSaving_txn(const std::string &arg, const unsigned &amount,
                           const std::string &endpoint, const std::string &from_address,
                           const std::string &to_address) {
  
  std::string request_json = SEND_TXN_PREFIX + from_address + MIDDLE_PART_1 + to_address +
         MIDDLE_PART_2 + encode_updateSaving(arg, amount) + SEND_TXN_SUFFIX;
  //std::cout << "request_json:" << request_json << std::endl;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, request_json);
  //std::cout << "result:" << r << std::endl;
  return get_json_field(r, "result");
}

inline std::string submit_writeCheck_txn(const std::string &arg, const unsigned &amount,
                           const std::string &endpoint, const std::string &from_address,
                           const std::string &to_address) {
  
  std::string request_json = SEND_TXN_PREFIX + from_address + MIDDLE_PART_1 + to_address +
         MIDDLE_PART_2 + encode_writeCheck(arg, amount) + SEND_TXN_SUFFIX;
  //std::cout << "request_json:" << request_json << std::endl;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, request_json);
  //std::cout << "result:" << r << std::endl;
  return get_json_field(r, "result");
}

inline std::string submit_sendPayment_txn(const std::string &arg0, const std::string &arg1, const unsigned &amount,
                           const std::string &endpoint, const std::string &from_address,
                           const std::string &to_address) {
  
  std::string request_json = SEND_TXN_PREFIX + from_address + MIDDLE_PART_1 + to_address +
         MIDDLE_PART_2 + encode_sendPayment(arg0, arg1, amount) + SEND_TXN_SUFFIX;
  //std::cout << "request_json:" << request_json << std::endl;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, request_json);
  //std::cout << "result:" << r << std::endl;
  return get_json_field(r, "result");
}

inline std::vector<std::string> poll_txs_by_block_hash(const std::string &endpoint,
                                                std::string block_hash) {
  std::string request =
      GET_BLOCK_BY_HASH_PREFIX + block_hash + GET_BLOCK_BY_HASH_SUFFIX;
  auto r = send_jsonrpc_request(endpoint, REQUEST_HEADERS, request);
  auto ret = get_list_field(r, "transactions");
  return ret;
}

inline std::vector<std::string> poll_txs_by_block_number(const std::string &endpoint,
                                                  int block_number) {
  std::string request = GET_BLOCK_BY_NUMBER_PREFIX +
                        std::to_string(block_number) +
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


#endif
