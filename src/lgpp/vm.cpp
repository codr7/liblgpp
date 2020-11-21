#include "vm.hpp"

namespace lgpp {

  const Op &VM::eval(PC start_pc, Stack &stack) { return eval(*(ops.data()+start_pc), stack); }
  
  const Op &VM::eval(const Op start_op, Stack &stack) {
    const Op *op = &start_op, *pop = op;
    while ((pop = op) && (op = op->eval(stack)));
    return *pop;    
  }
  
}
