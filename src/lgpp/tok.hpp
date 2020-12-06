#ifndef LGPP_TOK_HPP
#define LGPP_TOK_HPP

#include <ostream>

#include "lgpp/pos.hpp"

namespace lgpp {
  struct Tok;

  namespace toks {
    template <typename T>
    void dump(const Tok& tok, const T& imp, ostream& out);
  }

  struct Tok {
    struct Imp {
      virtual ~Imp() = default;
      virtual void dump(const Tok& tok, ostream& out) const = 0;
    };

    template <typename T>
    struct TImp: Imp {
      TImp(T imp): imp(move(imp)) { }

      void dump(const Tok& tok, ostream& out) const override { toks::dump(tok, imp, out); }

      T imp;
    };

    template <typename T>
    Tok(Pos pos, T imp): pos(pos), imp(make_shared<TImp<T>>(move(imp))) { } 

    template <typename T>
    const T& as() { return dynamic_cast<const TImp<T>&>(*imp).imp; }
    
    void dump(ostream& out) const { return imp->dump(*this, out); }

    const Pos pos;
    shared_ptr<const Imp> imp;
  };

  inline ostream &operator<<(ostream &out, const Tok &t) {
    t.imp->dump(t, out);
    return out;
  }
}

#endif
