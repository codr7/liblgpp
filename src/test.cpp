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
#include "lgpp/ops/join.hpp"
#include "lgpp/ops/pause.hpp"
#include "lgpp/ops/pop_stack.hpp"
#include "lgpp/ops/push.hpp"
#include "lgpp/ops/push_stack.hpp"
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
#include "lgpp/ops/unzip.hpp"
#include "lgpp/ops/zip.hpp"
#include "lgpp/parser.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/timer.hpp"
#include "lgpp/type.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

void parse_tests() {
  Parser p("parse_tests");
  p.alts.push_front(parse_group('(', ')'));
		   
  //parse(p, "foo + bar = 42");
  //for (auto &t: p.toks) { cout << t << endl; }
  assert(parse(p, "foo + bar = 42") == 5);
  assert(pop(p).as<toks::Id>().name == "foo");
  assert(pop(p).as<toks::Id>().name == "+");
  assert(pop(p).as<toks::Id>().name == "bar");
  assert(pop(p).as<toks::Id>().name == "=");
  assert(pop(p).as<toks::Lit>().val.as(types::Int) == 42);
}

void type_tests() {
  assert(isa(types::Int, types::Num) == &types::Num);
  assert(isa(types::Int, types::Seq) == &types::Seq);
}

void vm_branch_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 1);
  Label target("target");
  emit<ops::BranchEq>(vm, target, 0, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);

  target.pc = emit_pc(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0);
  
  assert(s.size() == 1);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();
}

void vm_call_tests(VM& vm) {
  auto& s = get_stack(vm);

  Label target("target");
  emit<ops::Call>(vm, target);
  emit<ops::Stop>(vm);
  target.pc = emit_pc(vm);
  emit<ops::Push>(vm, types::Int, 42);
  emit<ops::Ret>(vm);
  eval(vm, 0);
  
  assert(s.size() == 1);
  assert(pop(s, types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_compile_tests(VM &vm) {
  Parser p("compile_tests");
  parse(p, "foo 7");
  
  Env e;
  set(e, "foo", types::Int, 35);
  auto &t = get_thread(vm);
  compile(p, t, e);
  emit<ops::Add>(t);
  emit<ops::Stop>(t);
  
  auto& s = get_stack(t);
  eval(t, 0);
  assert(s.size() == 1);
  assert(pop(s, types::Int) == 42);
  t.ops.clear();
}

void vm_coro_tests(VM& vm) {
  auto& s = get_stack(vm);

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
  
  eval(vm, start_pc);
  assert(s.size() == 3);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();
}

void vm_inc_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 35);
  emit<ops::Inc>(vm, types::Int, 7);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_dec_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 49);
  emit<ops::Dec>(vm, types::Int, 7);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_add_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 35);
  emit<ops::Push>(vm, types::Int, 7);
  emit<ops::Add>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_sub_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 49);
  emit<ops::Push>(vm, types::Int, 7);
  emit<ops::Sub>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_math_tests(VM& vm) {
  vm_inc_tests(vm);
  vm_dec_tests(vm);
  vm_add_tests(vm);
  vm_sub_tests(vm);
}

void vm_stack_cp_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Push>(vm, types::Int, 3);
  emit<ops::Cp>(vm, 2, 2);
  emit<ops::Stop>(vm);
  eval(vm, 0);
  
  assert(s.size() == 5);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_drop_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Push>(vm, types::Int, 3);
  emit<ops::Drop>(vm, 1, 2);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_splat_tests(VM& vm) {
  auto& s = get_stack(vm);

  Stack v{{types::Int, 1}, {types::Int, 2}, {types::Int, 3}};
  emit<ops::Push>(vm, types::Stack, v);
  emit<ops::Splat>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0); 
  assert(s.size() == 3);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_squash_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Push>(vm, types::Int, 3);
  emit<ops::Squash>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0); 
  assert(s.size() == 1);
  s = pop(s, types::Stack);
  assert(s.size() == 3);
  assert(pop(s, types::Int) == 3);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_swap_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Swap>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0);
  assert(s.size() == 2);
  assert(pop(s, types::Int) == 1);
  assert(pop(s, types::Int) == 2);
  get_thread(vm).ops.clear();
}

void vm_push_stack_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::PushStack>(vm);
  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::PopStack>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0);
  s = pop(s, types::Stack);
  assert(s.size() == 2);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();
}

void vm_zip_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::Push>(vm, types::Int, 2);
  emit<ops::Zip>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0);
  assert(s.size() == 1);
  auto v = pop(s);
  assert(&get_type(v) == &types::Pair);
  auto p = v.as(types::Pair);
  assert(p.first.as(types::Int) == 1 && p.second.as(types::Int) == 2);
  get_thread(vm).ops.clear();
}

void vm_unzip_tests(VM& vm) {
  auto& s = get_stack(vm);
  Pair p({types::Int, 1}, {types::Int, 2});
  
  emit<ops::Push>(vm, types::Pair, p);
  emit<ops::Unzip>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0);
  assert(s.size() == 2);
  assert(pop(s, types::Int) == 2);
  assert(pop(s, types::Int) == 1);
  get_thread(vm).ops.clear();
}

void vm_pair_tests(VM& vm) {
  vm_zip_tests(vm);
  vm_unzip_tests(vm);
}

void vm_stack_tests(VM& vm) {
  vm_stack_cp_tests(vm);
  vm_stack_drop_tests(vm);
  vm_stack_splat_tests(vm);
  vm_stack_squash_tests(vm);
  vm_stack_swap_tests(vm);
  
  vm_push_stack_tests(vm);
}

void vm_thread_tests(VM& vm) {
  auto& s = get_stack(vm);

  Label target("target", emit_pc(vm));
  emit<ops::Push>(vm, types::Int, 42);
  emit<ops::Stop>(vm);

  auto start_pc = emit_pc(vm);
  emit<ops::StartThread>(vm, target);
  emit<ops::Join>(vm);
  emit<ops::Stop>(vm);
  
  eval(vm, start_pc);
  assert(s.size() == 1);
  s = pop(s, types::Stack);
  assert(s.size() == 1);
  assert(pop(s, types::Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_type_of_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Int, 1);
  emit<ops::TypeOf>(vm);
  emit<ops::Cp>(vm);
  emit<ops::TypeOf>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0);
  
  assert(s.size() == 2);
  assert(pop(s, types::Meta) == &types::Meta);
  assert(pop(s, types::Meta) == &types::Int);
  get_thread(vm).ops.clear();
}

void vm_isa_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, types::Meta, &types::Int);
  emit<ops::Push>(vm, types::Meta, &types::Num);
  emit<ops::Isa>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0);
  
  assert(s.size() == 1);
  assert(pop(s, types::Meta) == &types::Num);
  get_thread(vm).ops.clear();
}

void vm_type_tests(VM& vm) {
  vm_type_of_tests(vm);
  vm_isa_tests(vm);
}

void vm_tests() {
  VM vm;  
  vm_branch_tests(vm);
  vm_call_tests(vm);
  vm_compile_tests(vm);
  vm_coro_tests(vm);
  vm_math_tests(vm);
  vm_pair_tests(vm);
  vm_stack_tests(vm);
  vm_thread_tests(vm);
  vm_type_tests(vm);
}

int main() {
  parse_tests();
  type_tests();
  vm_tests();
  return 0;
}
