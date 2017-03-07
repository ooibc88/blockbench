#ifndef YCSB_C_STATISTIC_H_
#define YCSB_C_STATISTIC_H_

#include <fstream>

class Statistic {
 public:
  static Statistic* GetInstance(const std::string& stat_path,
                                const unsigned int checkpoint_interval) {
    static Statistic stat_(stat_path, checkpoint_interval);
    return &stat_;
  }
  void Send(const std::string& message) {
     if (os_.is_open()) {
       os_ << message << "\n";
       os_.flush();
     }
  }
  unsigned int GetInterval() const { return checkpoint_interval_; }

  ~Statistic() {
    if (os_.is_open()) os_.close();
  }

 private:
  Statistic(const std::string& stat_path,
            const unsigned int checkpoint_interval)
      : stats_path_(stat_path), checkpoint_interval_(checkpoint_interval) {
    if (os_.is_open()) os_.close();
    os_.open(stat_path.c_str(), std::ios::app);
  }
  std::string stats_path_;
  std::ofstream os_;
  unsigned int checkpoint_interval_;
};

#endif  // YCSB_C_SATISTIC_H_
