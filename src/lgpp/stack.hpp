#ifndef LGPP_STACK_HPP
#define LGPP_STACK_HPP

#include <vector>
#include "val.hpp"

namespace lgpp {

  using namespace std;
  
  using Stack = vector<Val>;

  inline void push(Stack &stack, Val val) { stack.push_back(val); }

  template <typename T, typename...Args>
  void push(Stack& stack, Type<T>& type, Args&&...args) {
    stack.push_back(Val(type, T(forward<Args>(args)...)));
  }

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
