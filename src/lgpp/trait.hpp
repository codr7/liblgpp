#ifndef LGPP_TRAIT_HPP
#define LGPP_TRAIT_HPP

#include <map>
#include <string>

namespace lgpp {
  using namespace std;

  struct Trait;
  
  void derive(Trait &child, Trait &parent, Trait &root);

  struct Trait {
    Trait(VM& vm, string name, initializer_list<Trait *> parents = {}): vm(vm), name(name) {
      for (auto p: parents) { derive(*this, *p, *p); }
    }
    
    Trait(const Trait &) = delete;
    Trait &operator =(const Trait &) = delete;

    VM& vm;
    const string name;
    map<Trait *, Trait *> isa;
  };

  inline void derive(Trait &child, Trait &parent, Trait &root) {
    child.isa.insert(make_pair(&parent, &root));
    for (const auto &[p, r]: parent.isa) { derive(child, *p, *r); }
  }

  inline Trait *isa(Trait &child, Trait &parent) {
    auto found = child.isa.find(&parent);
    if (found == child.isa.end()) { return nullptr; }
    return found->second;
  }
}

#endif
