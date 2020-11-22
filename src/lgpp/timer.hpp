#ifndef LGPP_TIMER_HPP
#define LGPP_TIMER_HPP

#include <chrono>

namespace lgpp {

  using namespace std;
  
  struct Timer {
    using Clock = chrono::steady_clock;
    using Time = Clock::time_point;

    void reset() { started = Clock::now(); }

    auto ns() { return chrono::duration_cast<chrono::nanoseconds>(Clock::now() - started).count(); }

    auto us() { return chrono::duration_cast<chrono::microseconds>(Clock::now() - started).count(); }

    auto ms() { return chrono::duration_cast<chrono::milliseconds>(Clock::now() - started).count(); }

    Time started;
  };
    
}

#endif
