
#ifndef OLTP_C_SPINLOCK_H_
#define OLTP_C_SPINLOCK_H_

#include <atomic>

class SpinLock {
  std::atomic_flag locked = ATOMIC_FLAG_INIT;

  public:
  void lock() {
    while (locked.test_and_set(std::memory_order_acquire)) {
      ;
    }
  }
  void unlock() { locked.clear(std::memory_order_release); }
};
#endif // OLTP_C_UTILS_H_