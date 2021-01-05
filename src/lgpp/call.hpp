#ifndef LGPP_CALL_HPP
#define LGPP_CALL_HPP

#include "lgpp/pc.hpp"

namespace lgpp {
  struct Call {
    enum class Opts {NONE = 0, CORO = 1};

    Call(PC pc, PC return_pc, Opts opts = Opts::NONE): pc(pc), return_pc(return_pc), opts(opts) {}

    PC pc, return_pc;
    Opts opts;
  };
}

#endif
