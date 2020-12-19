#ifndef LGPP_MACRO_HPP
#define LGPP_MACRO_HPP

#include "lgpp/env.hpp"
#include "lgpp/types.hpp"

namespace lgpp {
  struct Parser;
  struct Thread;
  
  struct Macro {
    using Imp = function<void (Parser &, Thread &, Env &)>;

    Macro(string name, Imp imp): name(name), imp(imp) {}

    const string name;
    Imp imp;
  };

}

#endif
