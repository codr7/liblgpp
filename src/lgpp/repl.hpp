#ifndef LGPP_REPL_HPP
#define LGPP_REPL_HPP

#include <iostream>
#include <optional>
#include <sstream>

namespace lgpp {

  using namespace std;
  
  struct REPL {
    REPL(istream &in, ostream &out): in(in), out(out) {}

    istream &in;
    ostream &out;

    function<bool (const string &line)> on_getline;
  };
  
  inline void enter(REPL &repl) {
    stringstream buf;
    string line;
  
    while (getline(repl.in, line)) {
      if (repl.on_getline && !repl.on_getline(line)) { break; }
    }
  }

}

#endif
