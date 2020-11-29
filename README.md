This project implements an extensible stack based VM for interpreters in the form of a C++17 library.

```
#include <cassert>

#include "lgpp/ops/push.hpp"
#include "lgpp/ops/stop.hpp"
#include "lgpp/stack.hpp"
#include "lgpp/vm.hpp"

using namespace lgpp;

int main() {
  VM vm;
  Stack s;

  vm.emit<ops::Push>(types::Int, 42);
  vm.emit<ops::Stop>();

  vm.eval(0, s);
  assert(pop(s, types::Int) == 42);

  return 0;
}
```

Custom interpreters are powerful and flexible tools that allow solving thorny problems with grace.

The general idea is that distilling the fundamental building blocks in library form makes it possible to reduce needed effort to the point where more problems start to look like custom languages, and where it's affordable to try out new ideas and throw some away.

The provided VM so far supports four [types](https://github.com/codr7/liblgpp/blob/main/src/lgpp/types.hpp) of values: coroutines, threads, stacks and integers; and the minimal set of [operations](https://github.com/codr7/liblgpp/tree/main/src/lgpp/ops) needed to write simple algorithms; but it is trivial to extend with additional types and operations.

### setup
The project requires a C++17 compiler and CMake to build.

```
$ git clone https://github.com/codr7/liblgpp.git
$ cd liblgpp
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./tests
fibrec: 762023us
coro: 566445us
thread: 1002093us
```

### performance
Single threaded performance is around 3-5 times slower than [Python3](https://github.com/codr7/liblgpp/blob/main/bench/fibrec.py). While I believe that's possible to improve significantly within the constraints imposed by the current design, in the end [computed goto](https://github.com/codr7/liblg) is going to outperform function calls for the [core evaluation loop](https://github.com/codr7/liblgpp/blob/f5eba0b60a65da2c6c7eea60e42a752b1843999f/src/lgpp/vm.hpp#L33).

```
$ cd bench
$ python3 fibrec.py
238096us
$ python3 coro.py
159311us
```

### design
Modern C++ is a fairly complex language, but it is unique in the way it allows dialling in just the right balance between abstraction, performance and safety. I've tried my best to keep the code as simple as straight forward as the language allows, using the standard library where appropriate without getting tangled up.

One somewhat unusual aspect of the design is that it doesn't use inheritance for [polymorphism](https://github.com/codr7/liblgpp/blob/main/src/lgpp/op.hpp), [this](https://www.youtube.com/watch?v=QGcVXgEVMJg) allows more flexibility in extending the library and simplifies usage.

The [core evaluation loop](https://github.com/codr7/liblgpp/blob/f5eba0b60a65da2c6c7eea60e42a752b1843999f/src/lgpp/vm.hpp#L33) trades speed for simplicity, extensibility, portability and maintainability by dispatching to functions rather than using computed goto and representing operations as structs rather than packed bytecode.

### coroutines
Coroutines are labels that support pausing/resuming calls. Since they are passed by value, copying results in a separate call chain starting at the same position.

```
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
assert(pop(s, types::Int) == 3);
assert(pop(s, types::Int) == 2);
assert(pop(s, types::Int) == 1);
```

### threads
The VM supports preemptive multithreading. Each thread runs in complete isolation on its own stack, which is pushed to the calling stack when joining.

```
Label target("target", vm.emit_pc());
vm.emit<ops::Push>(types::Int, 1000);
vm.emit<ops::Sleep>();
vm.emit<ops::Stop>();

auto start_pc = vm.emit_pc();
vm.emit<ops::StartThread>(target);
vm.emit<ops::Join>();
assert(pop(s, types::Stack).size() == 0);
vm.emit<ops::Stop>();
  
vm.eval(start_pc, s);
```