#include "vm.hpp"

namespace lgpp {

  const Op &VM::eval(PC start_pc, Stack &stack) { return eval(*(thread().ops.data()+start_pc), stack); }
  
  const Op &VM::eval(const Op &start_op, Stack &stack) {
    const Op *pop = nullptr;
    for (const Op *op = &start_op; op; pop = op, op = op->eval(stack));
    return *pop;    
  }
  
}
