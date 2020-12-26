#include "lgpp/thread.hpp"
#include "lgpp/types.hpp"

namespace lgpp::types {
  Trait Any("Any"), Num("Num"), Seq("Seq");
  
  Type<bool> Bool("Bool", {&Any});

  Type<lgpp::Coro> Coro("Coro", {&Any, &Seq});

  Type<int> Int("Int", {&Num, &Seq});

  Type<lgpp::Label *> Label("Label", {&Any});

  Type<lgpp::Macro> Macro("Macro", {&Any});

  Type<lgpp::Trait *> Meta("Meta", {&Any});

  Type<nullptr_t> Nil("Nil");

  Type<lgpp::Thread::Id> Thread("Thread", {&Any});

  Type<pair<Val, Val>> Pair("Pair", {&Any});

  Type<lgpp::Prim> Prim("Prim", {&Any});

  Type<lgpp::Stack> Stack("Stack", {&Any, &Seq});
}
