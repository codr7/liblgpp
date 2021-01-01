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
#include "lgpp/ops/return.hpp"
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

void vm_parse_tests(VM& vm) {
  Parser p(vm, "parse_tests");
  p.alts.push_back(parse_int);
  p.alts.push_back(parse_group('(', ')'));
  p.alts.push_back(parse_id);
		   
  parse(p, "foo + bar = 42");
  assert(pop(p).as<toks::Id>().name == "foo");
  assert(pop(p).as<toks::Id>().name == "+");
  assert(pop(p).as<toks::Id>().name == "bar");
  assert(pop(p).as<toks::Id>().name == "=");
  assert(pop(p).as<toks::Lit>().val.as(vm.Int) == 42);
}

void vm_branch_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 1);
  Label target("target");
  emit<ops::BranchEq>(vm, target, 0, vm.Int, 1);
  emit<ops::Push>(vm, vm.Int, 2);

  target.pc = emit_pc(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0);
  
  assert(s.size() == 1);
  assert(pop(s, vm.Int) == 1);
  get_thread(vm).ops.clear();
}

void vm_call_tests(VM& vm) {
  auto& s = get_stack(vm);

  Label target("target");
  emit<ops::Call>(vm, vm.Label, &target);
  emit<ops::Stop>(vm);
  target.pc = emit_pc(vm);
  emit<ops::Push>(vm, vm.Int, 42);
  emit<ops::Return>(vm);
  eval(vm, 0);
  
  assert(s.size() == 1);
  assert(pop(s, vm.Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_compile_tests(VM &vm) {
  Parser p(vm, "compile_tests");
  p.alts.push_back(parse_int);
  p.alts.push_back(parse_id);
  parse(p, "foo 7");
  
  Env e(vm);
  let(e, "foo", vm.Int, 35);
  auto &t = get_thread(vm);
  compile(p, t, e);
  emit<ops::Add>(t);
  emit<ops::Stop>(t);
  
  auto& s = get_stack(t);
  eval(t, 0);
  assert(s.size() == 1);
  assert(pop(s, vm.Int) == 42);
  t.ops.clear();
}

void vm_coro_tests(VM& vm) {
  auto& s = get_stack(vm);

  Label target("target", emit_pc(vm));
  emit<ops::Push>(vm, vm.Int, 1);
  emit<ops::Push>(vm, vm.Int, 2);
  emit<ops::Pause>(vm);
  emit<ops::Push>(vm, vm.Int, 3);
  emit<ops::Return>(vm);
  
  auto start_pc = emit_pc(vm);
  emit<ops::StartCoro>(vm, target);
  emit<ops::Resume>(vm);
  emit<ops::Resume>(vm);
  emit<ops::Drop>(vm);
  emit<ops::Stop>(vm);
  
  eval(vm, start_pc);
  assert(s.size() == 3);
  assert(pop(s, vm.Int) == 3);
  assert(pop(s, vm.Int) == 2);
  assert(pop(s, vm.Int) == 1);
  get_thread(vm).ops.clear();
}

void vm_inc_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 35);
  emit<ops::Inc>(vm, vm.Int, 7);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, vm.Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_dec_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 49);
  emit<ops::Dec>(vm, vm.Int, 7);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, vm.Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_add_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 35);
  emit<ops::Push>(vm, vm.Int, 7);
  emit<ops::Add>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, vm.Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_sub_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 49);
  emit<ops::Push>(vm, vm.Int, 7);
  emit<ops::Sub>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, vm.Int) == 42);
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

  emit<ops::Push>(vm, vm.Int, 1);
  emit<ops::Push>(vm, vm.Int, 2);
  emit<ops::Push>(vm, vm.Int, 3);
  emit<ops::Cp>(vm, 2, 2);
  emit<ops::Stop>(vm);
  eval(vm, 0);
  
  assert(s.size() == 5);
  assert(pop(s, vm.Int) == 2);
  assert(pop(s, vm.Int) == 1);
  assert(pop(s, vm.Int) == 3);
  assert(pop(s, vm.Int) == 2);
  assert(pop(s, vm.Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_drop_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 1);
  emit<ops::Push>(vm, vm.Int, 2);
  emit<ops::Push>(vm, vm.Int, 3);
  emit<ops::Drop>(vm, 1, 2);
  emit<ops::Stop>(vm);
  eval(vm, 0); 

  assert(s.size() == 1);
  assert(pop(s, vm.Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_splat_tests(VM& vm) {
  auto& s = get_stack(vm);

  Stack v{{vm.Int, 1}, {vm.Int, 2}, {vm.Int, 3}};
  emit<ops::Push>(vm, vm.Stack, v);
  emit<ops::Splat>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0); 
  assert(s.size() == 3);
  assert(pop(s, vm.Int) == 3);
  assert(pop(s, vm.Int) == 2);
  assert(pop(s, vm.Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_squash_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 1);
  emit<ops::Push>(vm, vm.Int, 2);
  emit<ops::Push>(vm, vm.Int, 3);
  emit<ops::Squash>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0); 
  assert(s.size() == 1);
  s = pop(s, vm.Stack);
  assert(s.size() == 3);
  assert(pop(s, vm.Int) == 3);
  assert(pop(s, vm.Int) == 2);
  assert(pop(s, vm.Int) == 1);
  get_thread(vm).ops.clear();  
}

void vm_stack_swap_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 1);
  emit<ops::Push>(vm, vm.Int, 2);
  emit<ops::Swap>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0);
  assert(s.size() == 2);
  assert(pop(s, vm.Int) == 1);
  assert(pop(s, vm.Int) == 2);
  get_thread(vm).ops.clear();
}

void vm_push_stack_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::PushStack>(vm);
  emit<ops::Push>(vm, vm.Int, 1);
  emit<ops::Push>(vm, vm.Int, 2);
  emit<ops::PopStack>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0);
  s = pop(s, vm.Stack);
  assert(s.size() == 2);
  assert(pop(s, vm.Int) == 2);
  assert(pop(s, vm.Int) == 1);
  get_thread(vm).ops.clear();
}

void vm_zip_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 1);
  emit<ops::Push>(vm, vm.Int, 2);
  emit<ops::Zip>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0);
  assert(s.size() == 1);
  auto v = pop(s);
  assert(&get_type(v) == &vm.Pair);
  auto p = v.as(vm.Pair);
  assert(p.first.as(vm.Int) == 1 && p.second.as(vm.Int) == 2);
  get_thread(vm).ops.clear();
}

void vm_unzip_tests(VM& vm) {
  auto& s = get_stack(vm);
  Pair p({vm.Int, 1}, {vm.Int, 2});
  
  emit<ops::Push>(vm, vm.Pair, p);
  emit<ops::Unzip>(vm);
  emit<ops::Stop>(vm);

  eval(vm, 0);
  assert(s.size() == 2);
  assert(pop(s, vm.Int) == 2);
  assert(pop(s, vm.Int) == 1);
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
  emit<ops::Push>(vm, vm.Int, 42);
  emit<ops::Stop>(vm);

  auto start_pc = emit_pc(vm);
  emit<ops::StartThread>(vm, target);
  emit<ops::Join>(vm);
  emit<ops::Stop>(vm);
  
  eval(vm, start_pc);
  assert(s.size() == 1);
  s = pop(s, vm.Stack);
  assert(s.size() == 1);
  assert(pop(s, vm.Int) == 42);
  get_thread(vm).ops.clear();
}

void vm_type_of_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Int, 1);
  emit<ops::TypeOf>(vm);
  emit<ops::Cp>(vm);
  emit<ops::TypeOf>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0);
  
  assert(s.size() == 2);
  assert(pop(s, vm.Meta) == &vm.Meta);
  assert(pop(s, vm.Meta) == &vm.Int);
  get_thread(vm).ops.clear();
}

void vm_isa_tests(VM& vm) {
  auto& s = get_stack(vm);

  emit<ops::Push>(vm, vm.Meta, &vm.Int);
  emit<ops::Push>(vm, vm.Meta, &vm.Num);
  emit<ops::Isa>(vm);
  emit<ops::Stop>(vm);
  eval(vm, 0);
  
  assert(s.size() == 1);
  assert(pop(s, vm.Bool));
  get_thread(vm).ops.clear();
}

void vm_type_tests(VM& vm) {
  assert(isa(vm.Int, vm.Num) == &vm.Num);
  assert(isa(vm.Int, vm.Seq) == &vm.Seq);

  vm_type_of_tests(vm);
  vm_isa_tests(vm);
}

int main() {
  VM vm;  
  vm_parse_tests(vm);

  vm_branch_tests(vm);
  vm_call_tests(vm);
  vm_compile_tests(vm);
  vm_coro_tests(vm);
  vm_math_tests(vm);
  vm_pair_tests(vm);
  vm_stack_tests(vm);
  vm_thread_tests(vm);
  vm_type_tests(vm);
  return 0;
}
