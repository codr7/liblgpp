#include "vm.hpp"

namespace lgpp {
  const Op &VM::eval(PC pc, Stack &stack) {
    const Op *op = ops.data()+pc, *pop = op;
    while ((pop = op) && (op = op->eval(stack)));
    return *pop;
  }
}
