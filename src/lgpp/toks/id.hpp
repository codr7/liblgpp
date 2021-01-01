#ifndef LGPP_TOKS_ID_HPP
#define LGPP_TOKS_ID_HPP

#include <lgpp/tok.hpp>
#include <lgpp/ops/push.hpp>

namespace lgpp::toks {
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
