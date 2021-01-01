#include "lgpp/vm.hpp"

namespace lgpp {
  VM::VM():
    Any(*this, "Any"), Num(*this, "Num"), Seq(*this, "Seq"),
    Bool(*this, "Bool", {&Any}),
    Coro(*this, "Coro", {&Any, &Seq}),
    Int(*this, "Int", {&Num, &Seq}),
    Label(*this, "Label", {&Any}),
    Macro(*this, "Macro", {&Any}),
    Meta(*this, "Meta", {&Any}),
    Pair(*this, "Pair", {&Any}),
    Prim(*this, "Prim", {&Any}),
    Stack(*this, "Stack", {&Any, &Seq}),
    Thread(*this, "Thread", {&Any}) {
    start_thread(*this, this_thread::get_id());
  }

  VM::~VM() {}
}
