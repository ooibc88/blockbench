//
//  string_hashtable.h
//
//  Created by Jinglei Ren on 12/22/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_LIB_STRING_HASHTABLE_H_
#define YCSB_C_LIB_STRING_HASHTABLE_H_

#include <vector>

namespace vmp {

template <class V>
class StringHashtable {
 public:
  typedef std::pair<const char *, V> KVPair;

  virtual V Get(const char *key) const = 0; ///< Returns NULL if not found
  virtual bool Insert(const char *key, V value) = 0;
  virtual V Update(const char *key, V value) = 0;
  virtual V Remove(const char *key) = 0;
  virtual std::vector<KVPair> Entries(const char *key = NULL,
                                      std::size_t n = -1) const = 0;
  virtual std::size_t Size() const = 0;

  virtual ~StringHashtable() { }
};

} // vmp

#endif // YCSB_C_LIB_STRING_HASHTABLE_H_

