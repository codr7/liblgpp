#ifndef LGPP_STACK_HPP
#define LGPP_STACK_HPP

#include <ostream>
#include <vector>
#include "val.hpp"

namespace lgpp {

  using namespace std;
  
  using Stack = vector<Val>;

  template <typename...Args>
  void push(Stack& stack, Args&&...args) { stack.emplace_back(forward<Args>(args)...); }

  inline Val pop(Stack& stack) {
    if (stack.empty()) { throw runtime_error("Stack is empty"); }
    Val v(stack.back());
    stack.pop_back();
    return v;
  } 

  template <typename T>
  T pop(Stack& stack, Type<T>& type) { return pop(stack).as(type); } 

  inline void dump(const Stack &stack, ostream &out) {
    out << '[';
    auto i = 0;
    
    for (auto v: stack) {
      if (i++) { out << ' '; }
      v.imp->dump(out);
    }
    
    out << ']';
  };

  inline ostream &operator<<(ostream &out, const Stack &s) {
    dump(s, out);
    return out;
  }
}

#endif
