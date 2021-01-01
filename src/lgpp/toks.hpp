#ifndef LGPP_TOKS_HPP
#define LGPP_TOKS_HPP

#include "lgpp/env.hpp"
#include "lgpp/ops/push.hpp"
#include "lgpp/thread.hpp"
#include "lgpp/tok.hpp"
#include "lgpp/val.hpp"

namespace lgpp::toks {
  using namespace lgpp;
  
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

  struct Id {
    Id(string name): name(name) {}
    string name;
  };

  template <>
  inline void compile(const Tok& tok, const Id& imp, Toque& in, Thread& out, Env& env) {
    VM& vm(env.vm);
    auto found = find(env, imp.name);
    if (!found) { throw ECompile(tok.pos, "Unknown identifier: ", imp.name); }
    Val v = *found;
    auto vt = &get_type(v);
    
    if (vt == &vm.Macro) {
      v.as(vm.Macro).imp(in, out, env);
    } else {
      emit<ops::Push>(out, tok.pos, v);
    }
  }

  template <>
  inline void dump(const Tok &tok, const Id &imp, ostream &out) {
    out << "(id " << imp.name << ')';
  }
}

#endif
