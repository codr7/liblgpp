#ifndef LGPP_RET_HPP
#define LGPP_RET_HPP

#include "lgpp/pc.hpp"

namespace lgpp {
  struct Ret {
    enum class Opts {NONE = 0, CORO = 1};

    Ret(PC pc, Opts opts = Opts::NONE): pc(pc), opts(opts) {}

    PC pc;
    Opts opts;
  };
}

#endif
