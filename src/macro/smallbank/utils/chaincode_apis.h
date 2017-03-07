#ifndef SMARTCONTRACT_DRIVER_UTILS_CHAINCODE_APIS_H_
#define SMARTCONTRACT_DRIVER_UTILS_CHAINCODE_APIS_H_

#include <string>
#include <vector>

const std::string REQUEST_HEADERS = "text/json";

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

#endif
