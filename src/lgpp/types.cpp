#include "lgpp/thread.hpp"
#include "lgpp/types.hpp"

namespace lgpp::types {
  Trait Num("Num"), Seq("Seq");
  
  Type<lgpp::Coro> Coro("Coro", {&Seq});

  Type<int> Int("Int", {&Num, &Seq});

  Type<lgpp::Trait *> Meta("Meta");

  Type<nullptr_t> Nil("Nil");

  Type<lgpp::Thread::Id> Thread("Thread");

  Type<pair<Val, Val>> Pair("Pair", {});

  Type<lgpp::Stack> Stack("Stack", {&Seq});
}
