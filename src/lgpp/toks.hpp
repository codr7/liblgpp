#ifndef LGPP_TOKS_HPP
#define LGPP_TOKS_HPP

#include "lgpp/tok.hpp"
#include "lgpp/val.hpp"

namespace lgpp::toks {
  struct Lit {
    template <typename...Args>
    Lit(Args &&...args): val(forward<Args>(args)...) {}
    Val val;
  };

  template <>
  inline void dump(const Tok &tok, const Lit &imp, ostream &out) {
    out << "(lit " << imp.val << ')';
  }

  struct Id {
    Id(string name): name(name) {}
    string name;
  };

  template <>
  inline void dump(const Tok &tok, const Id &imp, ostream &out) {
    out << "(id " << imp.name << ')';
  }
}

#endif
