#ifndef LGPP_STACK_HPP
#define LGPP_STACK_HPP

#include <vector>
#include "val.hpp"

namespace lgpp {

  using namespace std;
  
  using Stack = vector<Val>;

  inline void push(Stack &stack, Val val) { stack.push_back(val); }
  
  inline Val pop(Stack &stack) {
    if (!stack.size()) { throw runtime_error("Stack is empty"); }
    Val v(stack.back());
    stack.pop_back();
    return v;
  } 
}

#endif
