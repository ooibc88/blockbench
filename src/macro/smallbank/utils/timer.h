#ifndef SMARTCONTRACT_DRIVER_UTILS_TIMER_H_
#define SMARTCONTRACT_DRIVER_UTILS_TIMER_H_

#include <chrono>
#include <time.h>
#include <fcntl.h>

void sleep(double t);
long time_now();

template <typename T>
class Timer {
 public:
  void Start() { time_ = Clock::now(); }

  T End() {
    Duration span;
    Clock::time_point t = Clock::now();
    span = std::chrono::duration_cast<Duration>(t - time_);
    return span.count();
  }

  void Tic() { last_time_ = Clock::now(); }

  T Toc() {
    Duration span;
    Clock::time_point t = Clock::now();
    span = std::chrono::duration_cast<Duration>(t - last_time_);
    last_time_ = t;
    return span.count();
  }

 private:
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::duration<T> Duration;

  Clock::time_point time_;
  Clock::time_point last_time_;
};

#endif
