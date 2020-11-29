#ifndef LGPP_STACK_HPP
#define LGPP_STACK_HPP

#include <vector>
#include "val.hpp"

namespace lgpp {

  using namespace std;
  
  using Stack = vector<Val>;

  template <typename...Args>
  void push(Stack& stack, Args&&...args) { stack.emplace_back(forward<Args>(args)...); }

  inline Val pop(Stack& stack) {
    if (!stack.size()) { throw runtime_error("Stack is empty"); }
    Val v(stack.back());
    stack.pop_back();
    return v;
  } 

  template <typename T>
  T pop(Stack& stack, Type<T>& type) { return pop(stack).as(type); } 

}

#endif
