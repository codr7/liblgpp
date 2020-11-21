## Interpreters

The goal of this post is to implement a simple but practical stack based virtual machine for interpreters in C++.

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
  Val v;

  vm.emit<ops::Push>(v);
  // Using a separate Stop op simplifies the critical eval loop
  vm.emit<ops::Stop>();
  // Emitted operations are stored in a vector
  auto &stop(vm.ops.back());
  // .pc is the index into the ops vector
  assert(stop.pc == 1);
  // eval takes a start pc and returns the last evaluated op
  assert(&vm.eval(0, s) == &stop);
  // Check the push result
  assert(s.size() == 1);
  return 0;
}
```

Interpreters are useful tools for implementing all sorts of custom languages, from configuration languages to template processors to query languages, scripting languages and beyond.

Distilling the fundamental building blocks in library form makes it possible to reduce the needed effort to the point where more problems start to look like custom languages, and where you can afford to try out new ideas and throw some away.

I opted for a stack based VM rather than register based because of its relative simplicity.

The final VM supports two types of values, threads and integers; and the minimum number of operations needed to write simple algorithms using these features; but is easy to extend with additional types and operations.

I have tried to keep the code as simple as straight forward as modern C++ allows, using the standard library where appropriate without getting lost in abstractions. Modern C++ is a fairly complex language, some sort of prior experience is definitely recommended for maximum enjoyment.

### setup

```
```