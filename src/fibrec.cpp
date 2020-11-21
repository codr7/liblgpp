#include <cassert>

#include "lgpp/ops/push.hpp"
#include "lgpp/ops/stop.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

int main() {
  VM vm;
  Stack s;
  Val v;

  vm.emit<ops::Push>(v);
  vm.emit<ops::Stop>();
  auto &stop(vm.ops.back());
  assert(stop.pc == 1);
  assert(&vm.eval(0, s) == &stop); 
  assert(s.size() == 1);
  return 0;
}
