#ifndef LGPP_OP_HPP
#define LGPP_OP_HPP

#include <memory>
#include <utility>

#include "pc.hpp"
#include "stack.hpp"

namespace lgpp {
  using namespace std;

  struct Op;
  
  namespace ops {
    template <typename T>
    const Op *eval(const Op &op, const T &imp, Stack &stack);
  }
  
  struct Op {
    struct Imp {
      virtual ~Imp() = default;
      virtual const Op *eval(const Op &op, Stack &stack) const = 0;
    };

    template <typename T>
    struct TImp: Imp {
      TImp(T it): it(move(it)) { }

      const Op *eval(const Op &op, Stack &stack) const override { return ops::eval(op, it, stack); }

      T it;
    };

    template <typename T>
    Op(PC pc, T imp): pc(pc), imp(make_shared<TImp<T>>(move(imp))) { } 

    const Op *eval(Stack &stack) const { return imp->eval(*this, stack); }

    const PC pc;
    shared_ptr<const Imp> imp;
  };
}

#endif
