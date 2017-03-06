#include "timer.h"

namespace utils {

void sleep(double t){
  timespec req;
  req.tv_sec = (int) t; 
  req.tv_nsec = (int64_t)(1e9 * (t - (int64_t)t)); 
  nanosleep(&req, NULL); 
}

long time_now(){
  timespec ts; 
  clock_gettime(CLOCK_REALTIME, &ts); 
  return (ts.tv_sec*1000000000 + ts.tv_nsec); 
}
}

