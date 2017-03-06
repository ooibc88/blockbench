//
//  properties.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/9/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_PROPERTIES_H_
#define YCSB_C_PROPERTIES_H_

#include <string>
#include <map>
#include <fstream>
#include <cassert>
#include "utils.h"

namespace utils {

class Properties {
 public:
  std::string GetProperty(const std::string &key,
      const std::string &default_value = std::string()) const;
  void SetProperty(const std::string &key, const std::string &value);
  bool Load(std::ifstream &input);
  const std::map<std::string, std::string> &properties() const;
  std::string &operator[](const std::string &key) { return properties_[key]; }
 private:
  std::map<std::string, std::string> properties_;
};

inline std::string Properties::GetProperty(const std::string &key,
    const std::string &default_value) const {
  std::map<std::string, std::string>::const_iterator it = properties_.find(key);
  if (properties_.end() == it) {
    return default_value;
  } else return it->second;
}

inline void Properties::SetProperty(const std::string &key,
                                    const std::string &value) {
  properties_[key] = value;
}

inline bool Properties::Load(std::ifstream &input) {
  if (!input.is_open()) throw utils::Exception("File not open!");

  while (!input.eof() && !input.bad()) {
    std::string line;
    std::getline(input, line);
    if (line[0] == '#') continue;
    size_t pos = line.find_first_of('=');
    if (pos == std::string::npos) continue;
    SetProperty(Trim(line.substr(0, pos)), Trim(line.substr(pos + 1)));
  }
  return true;
}

inline const std::map<std::string, std::string> &Properties::properties() const {
  return properties_;
}

} // utils

#endif // YCSB_C_PROPERTIES_H_
