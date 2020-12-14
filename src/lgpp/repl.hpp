#ifndef LGPP_REPL_HPP
#define LGPP_REPL_HPP

#include <iostream>
#include <sstream>

namespace lgpp {

  using namespace std;
  
  void repl(istream &in, function<bool (const string &)> fn) {
    stringstream buf;
    string line;
  
    while (getline(in, line)) {
      if (!fn(line)) { break; }
    }
  }

}

#endif
