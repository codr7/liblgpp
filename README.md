The goal of this project is to implement a simple but practical stack based virtual machine for interpreters in C++; in the hope that the experience may generate enough insight to allow implementing similar ideas in any language, as well as a deeper understanding of what an interpreter is, how it works internally, and how to make them run fast enough for practical use. One reason that might be of interest is that most code today runs on top of multiple layers of interpreters: Java, .net, JavaScript and so on, running in some kind of virtual containers. Another reason is that custom interpreters are powerful and flexible tools that allow solving thorny problems with grace.

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
  Type<int> t("Int");

  vm.emit<ops::Push>(t, 42);
  vm.emit<ops::Stop>();
  vm.eval(0, s);
  assert(pop(s).as(t) == 42);
  return 0;
}
```

Distilling the fundamental building blocks in library form makes it possible to reduce needed effort to the point where more problems start to look like custom languages, and where it's affordable to try out new ideas and throw some away.

The VM supports two types of values, threads and integers; and the minimal set of operations needed to write simple algorithms; but it is trivial to extend with additional types and operations.

Modern C++ is a fairly complex language, but it is unique in how it allows dialling in just the right balance between abstraction, performance and safety. I've tried my best to keep the code as simple as straight forward as the language allows, using the standard library where appropriate without getting tangled up in needless abstraction.

### setup

```
```