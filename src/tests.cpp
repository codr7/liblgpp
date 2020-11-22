#include <cassert>

#include "lgpp/ops/push.hpp"
#include "lgpp/ops/stop.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/type.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

void vm_tests() {
  VM vm;
  Stack s;
  Type<int> t("Int");

  vm.emit<ops::Push>(t, 42);
  vm.emit<ops::Stop>();
  auto &stop(vm.last_op());
  assert(stop.pc == 1);
  assert(&vm.eval(0, s) == &stop); 
  assert(s.size() == 1);
  assert(s.back().as(t) == 42);
}

int main() {
  vm_tests();
  return 0;
}
