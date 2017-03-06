//
//  timer.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/19/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_TIMER_H_
#define YCSB_C_TIMER_H_

#include <chrono>
#include <iostream>
#include <ctime>
#include <time.h>
#include <fcntl.h>


namespace utils {

void sleep(double t); 
long time_now(); 

template <typename T>
class Timer {
 public:
  void Start() {
    time_ = Clock::now();
  }

  T End() {
    Duration span;
    Clock::time_point t = Clock::now();
    span = std::chrono::duration_cast<Duration>(t - time_);
    return span.count();
  }

  void Tic() {
    last_time_ = Clock::now();
  }

  T Toc() {
    Duration span;
    Clock::time_point t = Clock::now();
    span = std::chrono::duration_cast<Duration>(t - last_time_);
    /*
    auto llll = std::chrono::system_clock::to_time_t(last_time_);
    std::cout << "last time: " << ctime(&llll) << std::endl;
    auto lllll = std::chrono::system_clock::to_time_t(t);
    std::cout << "now time: " << ctime(&lllll) << std::endl;
    */

    last_time_ = t;
    return span.count();
  }

 private:
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::duration<T> Duration;

  Clock::time_point time_;
  Clock::time_point last_time_;
};

} // utils

#endif // YCSB_C_TIMER_H_

