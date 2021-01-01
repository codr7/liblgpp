#ifndef LGPP_TOKS_LIT_HPP
#define LGPP_TOKS_LIT_HPP

#include <lgpp/tok.hpp>
#include <lgpp/ops/push.hpp>

namespace lgpp::toks {

  struct Lit {
    template <typename...Args>
    Lit(Args &&...args): val(forward<Args>(args)...) {}
    Val val;
  };

  template <>
  inline void compile(const Tok& tok, const Lit& imp, Toque& in, Thread& out, Env& env) {
    emit<ops::Push>(out, tok.pos, imp.val);
  }

  template <>
  inline void dump(const Tok &tok, const Lit &imp, ostream &out) {
    out << "(lit " << imp.val << ')';
  }

}

#endif
