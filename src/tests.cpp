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
#include "lgpp/ops/isa.hpp"
#include "lgpp/ops/goto.hpp"
#include "lgpp/ops/join.hpp"
#include "lgpp/ops/pause.hpp"
#include "lgpp/ops/push.hpp"
#include "lgpp/ops/resume.hpp"
#include "lgpp/ops/ret.hpp"
#include "lgpp/ops/sleep.hpp"
#include "lgpp/ops/splat.hpp"
#include "lgpp/ops/squash.hpp"
#include "lgpp/ops/start_coro.hpp"
#include "lgpp/ops/start_thread.hpp"
#include "lgpp/ops/stop.hpp"
#include "lgpp/ops/sub.hpp"
#include "lgpp/ops/swap.hpp"
#include "lgpp/ops/type_of.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/timer.hpp"
#include "lgpp/type.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

void type_tests() {
  assert(isa(types::Int, types::Num) == &types::Num);
  assert(isa(types::Int, types::Seq) == &types::Seq);
}

void vm_branch_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 1);
  Label target("target");
  emit<ops::BranchEq>(vm, target, 0, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);

  target.pc = emit_pc(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0, s);
  
  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 1);
  get_thread(vm).ops.clear();
}

void vm_call_tests(VM& vm) {
  Stack s;

  Label target("target");
  emit<ops::Call>(vm, target);
  emit<ops::Stop>(vm);
  target.pc = emit_pc(vm);
  emit<ops::Push>(vm, types::Int, 42);
  emit<ops::Ret>(vm);
  eval(vm, 0, s);
  
  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_coro_tests(VM& vm) {
  Stack s;

  Label target("target", emit_pc(vm));
  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Pause>(vm);
  emit<ops::Push>(vm, types::Int, 3);
  emit<ops::Ret>(vm);
  
  auto start_pc = emit_pc(vm);
  emit<ops::StartCoro>(vm, target);
  emit<ops::Resume>(vm);
  emit<ops::Resume>(vm);
  emit<ops::Drop>(vm);
  emit<ops::Stop>(vm);
  
  eval(vm, start_pc, s);
  assert(s.size() == 3);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();
}

void vm_inc_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 35);
  emit<ops::Inc>(vm, types::Int, 7);
  emit<ops::Stop>(vm);
  eval(vm, 0, s); 

  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_dec_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 49);
  emit<ops::Dec>(vm, types::Int, 7);
  emit<ops::Stop>(vm);
  eval(vm, 0, s); 

  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_add_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 35);
  emit<ops::Push>(vm, types::Int, 7);
  emit<ops::Add>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0, s); 

  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_sub_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 49);
  emit<ops::Push>(vm, types::Int, 7);
  emit<ops::Sub>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0, s); 

  assert(s.size() == 1);
  assert(s.back().as(types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_math_tests(VM& vm) {
  vm_inc_tests(vm);
  vm_dec_tests(vm);
  vm_add_tests(vm);
  vm_sub_tests(vm);
}

void vm_stack_cp_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Push>(vm, types::Int, 3);
  emit<ops::Cp>(vm, 2, 2);
  emit<ops::Stop>(vm);
  eval(vm, 0, s);
  
  assert(s.size() == 5);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_drop_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Push>(vm, types::Int, 3);
  emit<ops::Drop>(vm, 1, 2);
  emit<ops::Stop>(vm);
  eval(vm, 0, s); 

  assert(s.size() == 1);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_splat_tests(VM& vm) {
  Stack s;
  Stack v{{types::Int, 1}, {types::Int, 2}, {types::Int, 3}};
  emit<ops::Push>(vm, types::Stack, v);
  emit<ops::Splat>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0, s); 
  assert(s.size() == 3);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_squash_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Push>(vm, types::Int, 3);
  emit<ops::Squash>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0, s); 
  assert(s.size() == 1);
  s = pop(s, types::Stack);
  assert(s.size() == 3);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_swap_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Swap>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0, s);
  assert(s.size() == 2);
  assert(pop(s, types::Int) == 1);
  assert(pop(s, types::Int) == 2);
  get_thread(vm).ops.clear();
}

void vm_stack_tests(VM& vm) {
  vm_stack_cp_tests(vm);
  vm_stack_drop_tests(vm);
  vm_stack_splat_tests(vm);
  vm_stack_squash_tests(vm);
  vm_stack_swap_tests(vm);
}

void vm_thread_tests(VM& vm) {
  Stack s;

  Label target("target", emit_pc(vm));
  emit<ops::Push>(vm, types::Int, 42);
  emit<ops::Stop>(vm);

  auto start_pc = emit_pc(vm);
  emit<ops::StartThread>(vm, target);
  emit<ops::Join>(vm);
  emit<ops::Stop>(vm);
  
  eval(vm, start_pc, s);
  assert(s.size() == 1);
  s = pop(s, types::Stack);
  assert(s.size() == 1);
  assert(pop(s, types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_type_of_tests(VM& vm) {
  Stack s;

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::TypeOf>(vm);
  emit<ops::Cp>(vm);
  emit<ops::TypeOf>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0, s);
  
  assert(s.size() == 2);
  assert(pop(s, types::Meta) == &types::Meta);
  assert(pop(s, types::Meta) == &types::Int);
  get_thread(vm).ops.clear();
}

void vm_isa_tests(VM& vm) {
  Stack s;
  emit<ops::Push>(vm, types::Meta, &types::Int);
  emit<ops::Push>(vm, types::Meta, &types::Num);
  emit<ops::Isa>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0, s);
  
  assert(s.size() == 1);
  assert(pop(s, types::Meta) == &types::Num);
  get_thread(vm).ops.clear();
}

void vm_type_tests(VM& vm) {
  vm_type_of_tests(vm);
  vm_isa_tests(vm);
}

void fibrec_bench(VM& vm) {
  Stack s;
  Label exit("exit");
  
  Label fib("fib", emit_pc(vm));
  emit<ops::BranchLt>(vm, exit, 0, types::Int, 2);
  emit<ops::Dec>(vm, types::Int, 1);
  emit<ops::Cp>(vm);
  emit<ops::Call>(vm, fib);
  emit<ops::Swap>(vm);
  emit<ops::Dec>(vm, types::Int, 1);
  emit<ops::Call>(vm, fib);
  emit<ops::Add>(vm);

  exit.pc = emit_pc(vm);
  emit<ops::Ret>(vm);

  PC start_pc = emit_pc(vm);
  emit<ops::Push>(vm, types::Int, 20);
  emit<ops::Call>(vm, fib);
  emit<ops::Stop>(vm);

  Timer timer;
  timer.reset();
  
  for (auto i = 0; i < 100; i++) {
    eval(vm, start_pc, s);
    assert(pop(s, types::Int) == 6765);
  }

  cout << "fibrec: " << timer.us() << "us" << endl;
  get_thread(vm).ops.clear();
}

void coro_bench(VM& vm) {
  Stack s;
  Label exit("exit");
  
  Label target("target", emit_pc(vm));
  emit<ops::BranchEq>(vm, exit, 0, types::Int, 0);
  emit<ops::Dec>(vm, types::Int, 1);
  emit<ops::Pause>(vm);
  emit<ops::Goto>(vm, target);
  exit.pc = emit_pc(vm);
  emit<ops::Ret>(vm);

  auto start_pc = emit_pc(vm);
  emit<ops::Push>(vm, types::Int, 1000000);
  emit<ops::StartCoro>(vm, target);
  
  Label loop("loop", emit_pc(vm));
  emit<ops::Resume>(vm);
  emit<ops::BranchGt>(vm, loop, 1, types::Int, 0);
  emit<ops::Stop>(vm);

  Timer timer;
  timer.reset();
  eval(vm, start_pc, s);
  cout << "coro: " << timer.us() << "us" << endl;
  get_thread(vm).ops.clear();
}

void thread_bench(VM& vm) {
  Stack s;
  auto ms = 1000;
  
  Label main("main", emit_pc(vm));
  emit<ops::Push>(vm, types::Int, ms);
  emit<ops::Sleep>(vm);
  emit<ops::Stop>(vm);

  auto start_pc = emit_pc(vm);
  emit<ops::StartThread>(vm, main);
  emit<ops::Push>(vm, types::Int, ms);
  emit<ops::Sleep>(vm);
  emit<ops::Join>(vm);
  emit<ops::Stop>(vm);

  Timer timer;
  timer.reset();
  eval(vm, start_pc, s);
  cout << "thread: " << timer.us() << "us" << endl;
  get_thread(vm).ops.clear();
}

void vm_tests() {
  VM vm;
  
  type_tests();
  
  vm_branch_tests(vm);
  vm_call_tests(vm);
  vm_coro_tests(vm);
  vm_math_tests(vm);
  vm_stack_tests(vm);
  vm_thread_tests(vm);
  vm_type_tests(vm);
  
  fibrec_bench(vm);
  coro_bench(vm);
  thread_bench(vm);
}

int main() {
  vm_tests();
  return 0;
}
