#ifndef LGPP_TOKS_GROUP_HPP
#define LGPP_TOKS_GROUP_HPP

#include <lgpp/tok.hpp>

namespace lgpp::toks {

  struct Group {
    template <typename...Args>
    Group(Args&&...args): toks(forward<Args>(args)...) {}
    vector<Tok> toks;
  };

  template <>
  inline void compile(const Tok& tok, const Group& imp, Toque& in, Thread& out, Env& env) {
    deque<Tok> ts(imp.toks.begin(), imp.toks.end());
    
    while (!ts.empty()) {
      auto t = ts.front();
      ts.pop_front();
      lgpp::compile(t, ts, out, env);
    }
  }

  template <>
  inline void dump(const Tok &tok, const Group &imp, ostream &out) {
    out << "(group";
    auto i = 0;

    for (auto &t: imp.toks) {
      if (i++) { out << ' '; }
      lgpp::dump(t, out);
    }
    
    out << ')';
  }

}

#endif
