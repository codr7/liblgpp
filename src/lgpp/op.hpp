#ifndef LGPP_OP_HPP
#define LGPP_OP_HPP

#include <memory>
#include <utility>

#include "pc.hpp"
#include "stack.hpp"

namespace lgpp {
  using namespace std;

  struct Op;
  struct Thread;
  
  namespace ops {
    template <typename T>
    const Op* eval(const Op& op, const T& imp, lgpp::Thread& thread);
  }
  
  struct Op {
    struct Imp {
      virtual ~Imp() = default;
      virtual const Op* eval(const Op& op, lgpp::Thread& thread) const = 0;
    };

    template <typename T>
    struct TImp: Imp {
      TImp(T imp): imp(move(imp)) { }

      const Op* eval(const Op& op, lgpp::Thread& thread) const override { return ops::eval(op, imp, thread); }

      T imp;
    };

    template <typename T>
    Op(PC pc, T imp): pc(pc), imp(make_shared<TImp<T>>(move(imp))) { } 

    template <typename T>
    const T& as() { return dynamic_cast<const TImp<T>&>(*imp).imp; }
    
    const PC pc;
    shared_ptr<const Imp> imp;
  };

  inline const Op *eval(const Op& op, lgpp::Thread& thread) { return op.imp->eval(op, thread); }

}

#endif
