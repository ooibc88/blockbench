//
//  skewed_latest_generator.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/9/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_SKEWED_LATEST_GENERATOR_H_
#define YCSB_C_SKEWED_LATEST_GENERATOR_H_

#include "generator.h"

#include <cstdint>
#include "counter_generator.h"
#include "zipfian_generator.h"

namespace ycsbc {

class SkewedLatestGenerator : public Generator<uint64_t> {
 public:
  SkewedLatestGenerator(CounterGenerator &counter) :
      basis_(counter), zipfian_(basis_.Last()) {
    Next();
  }
  
  uint64_t Next();
  uint64_t Last() { return last_; }
 private:
  CounterGenerator &basis_;
  ZipfianGenerator zipfian_;
  uint64_t last_;
};

inline uint64_t SkewedLatestGenerator::Next() {
  uint64_t max = basis_.Last();
  return last_ = max - zipfian_.Next(max);
}

} // ycsbc

#endif // YCSB_C_SKEWED_LATEST_GENERATOR_H_
