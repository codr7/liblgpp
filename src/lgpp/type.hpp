#ifndef LGPP_TYPE_HPP
#define LGPP_TYPE_HPP

#include <string>

namespace lgpp {
  using namespace std;
  
  template <typename T>
  struct Type {
    Type(string name): name(name) {}
    const string name;
  };
}

#endif
