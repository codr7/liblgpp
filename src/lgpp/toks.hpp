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
  inline void compile(const Tok& tok, const Group& imp, Parser& in, Thread& out, Env& env) {
    for (auto &t: imp.toks) { lgpp::compile(t, in, out, env); }
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
  inline void compile(const Tok& tok, const Lit& imp, Parser& in, Thread& out, Env& env) {
    emit<ops::Push>(out, imp.val);
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
  inline void compile(const Tok& tok, const Id& imp, Parser& in, Thread& out, Env& env) {
    auto found = env.find(imp.name);
    if (found == env.end()) { throw ECompile(tok.pos, "Unknown identifier: ", imp.name); }
    emit<ops::Push>(out, found->second);
  }

  template <>
  inline void dump(const Tok &tok, const Id &imp, ostream &out) {
    out << "(id " << imp.name << ')';
  }
}

#endif
