//
//  hash_string.h
//
//  Created by Jinglei Ren on 12/22/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_LIB_HASH_STRING_H_
#define YCSB_C_LIB_HASH_STRING_H_

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdint>

#include "lib/mem_alloc.h"

namespace vmp {

class String {
 public:
  String() : hash_(0), value_(NULL), len_(0) { }
  uint64_t hash() const { return hash_; }
  const char *value() const { return value_; }
  size_t length() const { return len_; }
  void set_value(const char *v);

  template <class Alloc>
  static String Copy(const char *v);

  static String Wrap(const char *v);

  template <class Alloc>
  static void Free(const String& str);

  bool operator==(const String &other) const;

 private:
  uint64_t SDBMHash(const char *str);

  uint64_t hash_;
  const char *value_;
  size_t len_;
};

inline void String::set_value(const char *v) {
  value_ = v;
  len_ = strlen(value_);
  hash_ = SDBMHash(value_);
}

inline uint64_t String::SDBMHash(const char *cstr) {
  uint64_t hash = 0;
  uint64_t c;
  while ((c = *cstr++) != '\0') {
    hash = c + (hash << 6) + (hash << 16) - hash;
  }
  return hash;
}

template <class Alloc>
inline String String::Copy(const char *cstr) {
  assert(cstr);
  String hstr;
  const size_t len = strlen(cstr); 
  char *str = (char *)Alloc::Malloc(len + 1);
  hstr.set_value(strcpy(str, cstr));
  assert(hstr.length() == len);
  return hstr;
}

inline String String::Wrap(const char *cstr) {
  assert(cstr);
  String hstr;
  hstr.set_value(cstr);
  return hstr;
}

template <class Alloc>
inline void String::Free(const String& hstr) {
  Alloc::Free(hstr.value(), hstr.length() + 1);
}

inline bool String::operator==(const String &other) const {
  if (hash_ != other.hash()) return false;
  return strcmp(value_, other.value()) == 0;
}

} // vmp

#endif // YCSB_C_LIB_HASH_STRING_H_

