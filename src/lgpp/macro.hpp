#ifndef LGPP_MACRO_HPP
#define LGPP_MACRO_HPP

#include "lgpp/env.hpp"
#include "lgpp/tok.hpp"

namespace lgpp {
  struct Parser;
  struct Thread;
  
  struct Macro {
    using Imp = function<void (Toque &, Thread &, Env &)>;

    Macro(string name, Imp imp): name(name), imp(imp) {}

    const string name;
    Imp imp;
  };

}

#endif
