#include <cassert>
#include <iostream>

#include "lgpp/label.hpp"
#include "lgpp/ops/add.hpp"
#include "lgpp/ops/branch_eq.hpp"
#include "lgpp/ops/branch_gt.hpp"
#include "lgpp/ops/branch_lt.hpp"
#include "lgpp/ops/call.hpp"
#include "lgpp/ops/cp.hpp"
#include "lgpp/ops/dec.hpp"
#include "lgpp/ops/drop.hpp"
#include "lgpp/ops/inc.hpp"
#include "lgpp/ops/goto.hpp"
#include "lgpp/ops/join.hpp"
#include "lgpp/ops/pause.hpp"
#include "lgpp/ops/push.hpp"
#include "lgpp/ops/resume.hpp"
#include "lgpp/ops/ret.hpp"
#include "lgpp/ops/sleep.hpp"
#include "lgpp/ops/start_coro.hpp"
#include "lgpp/ops/start_thread.hpp"
#include "lgpp/ops/stop.hpp"
#include "lgpp/ops/sub.hpp"
#include "lgpp/ops/swap.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/timer.hpp"
#include "lgpp/type.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

void vm_branch_tests(VM& vm) {
  Stack s;

  vm.thread().ops.reserve(10);
  vm.emit<ops::Push>(types::Int, 1);
  Label target("target");
  vm.emit<ops::BranchEq>(target, 0, types::Int, 1);
  vm.emit<ops::Push>(types::Int, 2);
  target.pc = vm.emit_pc();
  vm.emit<ops::Stop>();
  vm.eval(0, s);
  
  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 1);
  vm.clear_ops();
}

void vm_call_tests(VM& vm) {
  Stack s;

  Label target("target");
  vm.emit<ops::Call>(target);
  vm.emit<ops::Stop>();
  target.pc = vm.emit_pc();
  vm.emit<ops::Push>(types::Int, 42);
  vm.emit<ops::Ret>();
  vm.eval(0, s);
  
  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  vm.clear_ops();
}

void vm_inc_tests(VM& vm) {
  Stack s;

  vm.emit<ops::Push>(types::Int, 35);
  vm.emit<ops::Inc>(types::Int, 7);
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  vm.clear_ops();
}

void vm_dec_tests(VM& vm) {
  Stack s;

  vm.emit<ops::Push>(types::Int, 49);
  vm.emit<ops::Dec>(types::Int, 7);
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  vm.clear_ops();
}

void vm_add_tests(VM& vm) {
  Stack s;

  vm.emit<ops::Push>(types::Int, 35);
  vm.emit<ops::Push>(types::Int, 7);
  vm.emit<ops::Add>();
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  vm.clear_ops();
}

void vm_sub_tests(VM& vm) {
  Stack s;

  vm.emit<ops::Push>(types::Int, 49);
  vm.emit<ops::Push>(types::Int, 7);
  vm.emit<ops::Sub>();
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  vm.clear_ops();
}

void vm_math_tests(VM& vm) {
  vm_inc_tests(vm);
  vm_dec_tests(vm);
  vm_add_tests(vm);
  vm_sub_tests(vm);
}

void vm_stack_cp_tests(VM& vm) {
  Stack s;

  vm.emit<ops::Push>(types::Int, 1);
  vm.emit<ops::Push>(types::Int, 2);
  vm.emit<ops::Push>(types::Int, 3);
  vm.emit<ops::Cp>(2, 2);
  vm.emit<ops::Stop>();
  vm.eval(0, s);
  
  assert(s.size() == 5);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  vm.clear_ops();  
}

void vm_stack_drop_tests(VM& vm) {
  Stack s;

  vm.emit<ops::Push>(types::Int, 1);
  vm.emit<ops::Push>(types::Int, 2);
  vm.emit<ops::Push>(types::Int, 3);
  vm.emit<ops::Drop>(1, 2);
  vm.emit<ops::Stop>();
  vm.eval(0, s); 

  assert(s.size() == 1);
  assert(pop(s, types::Int) == 1);
  vm.clear_ops();  
}

void vm_stack_swap_tests(VM& vm) {
  Stack s;

  vm.emit<ops::Push>(types::Int, 1);
  vm.emit<ops::Push>(types::Int, 2);
  vm.emit<ops::Swap>();
  vm.emit<ops::Stop>();

  vm.eval(0, s);
  assert(s.size() == 2);
  assert(pop(s, types::Int) == 1);
  assert(pop(s, types::Int) == 2);
  vm.clear_ops();
}

void vm_stack_tests(VM& vm) {
  vm_stack_cp_tests(vm);
  vm_stack_drop_tests(vm);
  vm_stack_swap_tests(vm);
}

void vm_thread_tests(VM& vm) {
  Stack s;

  Label target("target", vm.emit_pc());
  vm.emit<ops::Push>(types::Int, 42);
  vm.emit<ops::Stop>();

  auto start_pc = vm.emit_pc();
  vm.emit<ops::StartThread>(target);
  vm.emit<ops::Join>();
  vm.emit<ops::Stop>();
  
  vm.eval(start_pc, s);
  assert(s.size() == 1);
  s = pop(s, types::Stack);
  assert(s.size() == 1);
  assert(pop(s, types::Int) == 42);
  vm.clear_ops();
}

void vm_coro_tests(VM& vm) {
  Stack s;

  Label target("target", vm.emit_pc());
  vm.emit<ops::Push>(types::Int, 1);
  vm.emit<ops::Push>(types::Int, 2);
  vm.emit<ops::Pause>();
  vm.emit<ops::Push>(types::Int, 3);
  vm.emit<ops::Ret>();
  
  auto start_pc = vm.emit_pc();
  vm.emit<ops::StartCoro>(target);
  vm.emit<ops::Resume>();
  vm.emit<ops::Resume>();
  vm.emit<ops::Drop>();
  vm.emit<ops::Stop>();
  
  vm.eval(start_pc, s);
  assert(s.size() == 3);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  vm.clear_ops();
}

void fibrec_bench(VM& vm) {
  Stack s;
  Label exit("exit");
  
  Label fib("fib", vm.emit_pc());
  vm.emit<ops::BranchLt>(exit, 0, types::Int, 2);
  vm.emit<ops::Dec>(types::Int, 1);
  vm.emit<ops::Cp>();
  vm.emit<ops::Call>(fib);
  vm.emit<ops::Swap>();
  vm.emit<ops::Dec>(types::Int, 1);
  vm.emit<ops::Call>(fib);
  vm.emit<ops::Add>();

  exit.pc = vm.emit_pc();
  vm.emit<ops::Ret>();

  PC start_pc = vm.emit_pc();
  vm.emit<ops::Push>(types::Int, 20);
  vm.emit<ops::Call>(fib);
  vm.emit<ops::Stop>();

  Timer timer;
  timer.reset();
  
  for (auto i = 0; i < 100; i++) {
    vm.eval(start_pc, s);
    assert(pop(s, types::Int) == 6765);
  }

  cout << "fibrec: " << timer.us() << "us" << endl;
  vm.clear_ops();
}

void coro_bench(VM& vm) {
  Stack s;
  Label exit("exit");
  
  Label target("target", vm.emit_pc());
  vm.emit<ops::BranchEq>(exit, 0, types::Int, 0);
  vm.emit<ops::Dec>(types::Int, 1);
  vm.emit<ops::Pause>();
  vm.emit<ops::Goto>(target);
  exit.pc = vm.emit_pc();
  vm.emit<ops::Ret>();

  auto start_pc = vm.emit_pc();
  vm.emit<ops::Push>(types::Int, 1000000);
  vm.emit<ops::StartCoro>(target);
  
  Label loop("loop", vm.emit_pc());
  vm.emit<ops::Resume>();
  vm.emit<ops::BranchGt>(loop, 1, types::Int, 0);
  vm.emit<ops::Stop>();

  Timer timer;
  timer.reset();
  vm.eval(start_pc, s);
  cout << "coro: " << timer.us() << "us" << endl;
  vm.clear_ops();
}

void thread_bench(VM& vm) {
  Stack s;
  auto ms = 1000;
  
  Label main("main", vm.emit_pc());
  vm.emit<ops::Push>(types::Int, ms);
  vm.emit<ops::Sleep>();
  vm.emit<ops::Stop>();

  auto start_pc = vm.emit_pc();
  vm.emit<ops::StartThread>(main);
  vm.emit<ops::Push>(types::Int, ms);
  vm.emit<ops::Sleep>();
  vm.emit<ops::Join>();
  vm.emit<ops::Stop>();

  Timer timer;
  timer.reset();
  vm.eval(start_pc, s);
  cout << "thread: " << timer.us() << "us" << endl;
  vm.clear_ops();
}

void vm_tests() {
  VM vm;
  
  vm_branch_tests(vm);
  vm_call_tests(vm);
  vm_math_tests(vm);
  vm_stack_tests(vm);
  vm_thread_tests(vm);
  vm_coro_tests(vm);
  
  fibrec_bench(vm);
  coro_bench(vm);
  thread_bench(vm);
}

int main() {
  vm_tests();
  return 0;
}
