//
//  const_generator.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/10/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_CONST_GENERATOR_H_
#define YCSB_C_CONST_GENERATOR_H_

#include "generator.h"
#include <cstdint>

namespace ycsbc {

class ConstGenerator : public Generator<uint64_t> {
 public:
  ConstGenerator(int constant) : constant_(constant) { }
  uint64_t Next() { return constant_; }
  uint64_t Last() { return constant_; }
 private:
  uint64_t constant_;
};

} // ycsbc

#endif // YCSB_C_CONST_GENERATOR_H_
