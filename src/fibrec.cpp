#include "lgpp/ops/stop.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

int main() {
  VM vm;
  Stack s;
  vm.emit<ops::Stop>();
  vm.eval(0, s);
  return 0;
}
