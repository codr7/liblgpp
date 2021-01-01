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
#include "lgpp/ops/go.hpp"
#include "lgpp/ops/join.hpp"
#include "lgpp/ops/pause.hpp"
#include "lgpp/ops/push.hpp"
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
#include "lgpp/stack.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/timer.hpp"
#include "lgpp/type.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

void fibrec_bench(VM& vm) {
  auto& s = get_stack(vm);
  Label exit("exit");
  
  Label fib("fib", emit_pc(vm));
  emit<ops::BranchLt>(vm, exit, 0, vm.Int, 2);
  emit<ops::Dec>(vm, vm.Int, 1);
  emit<ops::Cp>(vm);
  emit<ops::Call>(vm, vm.Label, &fib);
  emit<ops::Swap>(vm);
  emit<ops::Dec>(vm, vm.Int, 1);
  emit<ops::Call>(vm, vm.Label, &fib);
  emit<ops::Add>(vm);

  exit.pc = emit_pc(vm);
  emit<ops::Return>(vm);

  PC start_pc = emit_pc(vm);
  emit<ops::Push>(vm, vm.Int, 20);
  emit<ops::Call>(vm, vm.Label, &fib);
  emit<ops::Stop>(vm);

  Timer timer;
  timer.reset();
  
  for (auto i = 0; i < 100; i++) {
    eval(vm, start_pc);
    assert(pop(s, vm.Int) == 6765);
  }

  cout << "fibrec: " << timer.us() << "us" << endl;
  get_thread(vm).ops.clear();
}

void coro_bench(VM& vm) {
  Stack s;
  Label exit("exit");
  
  Label target("target", emit_pc(vm));
  emit<ops::BranchEq>(vm, exit, 0, vm.Int, 0);
  emit<ops::Dec>(vm, vm.Int, 1);
  emit<ops::Pause>(vm);
  emit<ops::Go>(vm, target);
  exit.pc = emit_pc(vm);
  emit<ops::Return>(vm);

  auto start_pc = emit_pc(vm);
  emit<ops::Push>(vm, vm.Int, 1000000);
  emit<ops::StartCoro>(vm, target);
  
  Label loop("loop", emit_pc(vm));
  emit<ops::Resume>(vm);
  emit<ops::BranchGt>(vm, loop, 1, vm.Int, 0);
  emit<ops::Stop>(vm);

  Timer timer;
  timer.reset();
  eval(vm, start_pc);
  cout << "coro: " << timer.us() << "us" << endl;
  get_thread(vm).ops.clear();
}

void thread_bench(VM& vm) {
  Stack s;
  auto ms = 1000;
  
  Label main("main", emit_pc(vm));
  emit<ops::Push>(vm, vm.Int, ms);
  emit<ops::Sleep>(vm);
  emit<ops::Stop>(vm);

  auto start_pc = emit_pc(vm);
  emit<ops::StartThread>(vm, main);
  emit<ops::Push>(vm, vm.Int, ms);
  emit<ops::Sleep>(vm);
  emit<ops::Join>(vm);
  emit<ops::Stop>(vm);

  Timer timer;
  timer.reset();
  eval(vm, start_pc);
  cout << "thread: " << timer.us() << "us" << endl;
  get_thread(vm).ops.clear();
}

int main() {
  VM vm;
  fibrec_bench(vm);
  coro_bench(vm);
  thread_bench(vm);
  return 0;
}
