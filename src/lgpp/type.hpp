#ifndef LGPP_TYPE_HPP
#define LGPP_TYPE_HPP

#include <string>

namespace lgpp {
  using namespace std;
  
  template <typename T>
  struct Type {
    Type(string name): name(name) {}
    Type(const Type &) = delete;
    Type &operator =(const Type &) = delete;
    const string name;
  };
}

#endif
