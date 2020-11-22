#include <cassert>

#include "lgpp/label.hpp"
#include "lgpp/ops/branch_eq.hpp"
#include "lgpp/ops/drop.hpp"
#include "lgpp/ops/push.hpp"
#include "lgpp/ops/stop.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/type.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

Type<int> Int("Int");

void vm_branch_tests(VM &vm) {
  Stack s;

  vm.thread().ops.reserve(10);
  vm.emit<ops::Push>(Int, 42);
  Label b;
  vm.emit<ops::BranchEq>(b, Int, 42);
  vm.emit<ops::Push>(Int, 7);
  vm.emit<ops::Stop>();
  b.pc = vm.last_op().pc;
  vm.eval(0, s); 
  assert(s.back().as(Int) == 42);
  vm.clear_ops();
}

void vm_stack_tests(VM &vm) {
  Stack s;

  vm.emit<ops::Push>(Int, 42);
  vm.emit<ops::Stop>();
  auto &stop(vm.last_op());
  assert(stop.pc == 1);
  assert(&vm.eval(0, s) == &stop); 
  assert(s.size() == 1);
  assert(s.back().as(Int) == 42);
  vm.clear_ops();
  
  vm.emit<ops::Drop>();
  vm.emit<ops::Stop>();
  vm.eval(0, s); 
  assert(s.size() == 0);
  vm.clear_ops();  
}

void vm_tests() {
  VM vm;
  
  vm_branch_tests(vm);
  vm_stack_tests(vm);
}

int main() {
  vm_tests();
  return 0;
}
