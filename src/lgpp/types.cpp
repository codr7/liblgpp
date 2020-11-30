#include "lgpp/thread.hpp"
#include "lgpp/types.hpp"

namespace lgpp::types {
  Trait Num("Num"), Seq("Seq");
  
  Type<lgpp::Coro> Coro("Coro", {&Seq});

  Type<int> Int("Int", {&Num, &Seq});

  Type<lgpp::Trait *> Meta("Meta");

  Type<lgpp::Thread::Id> Thread("Thread");

  Type<lgpp::Stack> Stack("Stack", {&Seq});
}
