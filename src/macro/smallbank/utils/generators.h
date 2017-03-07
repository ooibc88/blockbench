#ifndef SMARTCONTRACT_DRIVER_UTILS_GENERATORS_H_
#define SMARTCONTRACT_DRIVER_UTILS_GENERATORS_H_

#include <cstdint>
#include <string>
#include <random>

template <typename Value>
class Generator {
 public:
  virtual Value Next() = 0;
  virtual Value Last() = 0;
  virtual ~Generator() {}
};

class UniformGenerator : public Generator<uint64_t> {
 public:
  // Both min and max are inclusive
  UniformGenerator(uint64_t min, uint64_t max) : dist_(min, max) { Next(); }

  uint64_t Next() { return last_int_ = dist_(generator_); }
  uint64_t Last() { return last_int_; }

 private:
  uint64_t last_int_;
  std::mt19937_64 generator_;
  std::uniform_int_distribution<uint64_t> dist_;
};

#endif  // SMARTCONTRACT_DRIVERS_GENERATORS_H_
