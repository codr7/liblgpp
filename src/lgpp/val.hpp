#ifndef LGPP_VAL_HPP
#define LGPP_VAL_HPP

#include <memory>

#include "lgpp/pc.hpp"
#include "lgpp/pos.hpp"

namespace lgpp {  
  using namespace std;
  
  struct Thread;
  
  struct Trait;
  
  template <typename T>
  struct Type;

  struct Val;

  Trait& get_type(const Val &val);

  namespace types {

    template <typename T>
    PC call(Type<T>&, const T&, Thread&, PC, Pos);

    template <typename T>
    void dump(Type<T>&, const T&, ostream&);

    template <typename T>
    void say(Type<T>&, const T&, ostream&);

    template <typename T>
    bool is_true(Type<T>&, const T&);

    template <typename T>
    bool eq(Type<T>&, const T&, Val);

    template <typename T>
    bool gt(Type<T>&, const T&, Val);

    template <typename T>
    bool lt(Type<T>&, const T&, Val);

    template <typename T>
    Val add(Type<T>&, const T&, Val);

    template <typename T>
    Val sub(Type<T>&, const T&, Val);

  }

  struct Val {
    struct Imp {
      virtual ~Imp() = default;

      virtual Trait& type() const = 0;

      virtual PC call(Thread&, PC, Pos) const = 0;

      virtual void dump(ostream&) const = 0;
      virtual void say(ostream&) const = 0;
      
      virtual bool is_true() const = 0;
      virtual bool eq(Val) const = 0;
      virtual bool gt(Val) const = 0;
      virtual bool lt(Val) const = 0;

      virtual Val add(Val) const = 0;
      virtual Val sub(Val) const = 0;
    };

    template <typename T>
    struct TImp: Imp {
      TImp(Type<T>& type, T data): _type(type), data(move(data)) {}

      Trait& type() const override { return _type; }

      PC call(Thread& thread, PC pc, Pos pos) const override { return types::call(_type, data, thread, pc, pos); }
      void dump(ostream& out) const override { types::dump(_type, data, out); }
      void say(ostream& out) const override { types::say(_type, data, out); }
      bool is_true() const override { return types::is_true(_type, data); }
      bool eq(Val y) const override { return types::eq(_type, data, y); }
      bool gt(Val y) const override { return types::gt(_type, data, y); }
      bool lt(Val y) const override { return types::lt(_type, data, y); }

      Val add(Val y) const override { return types::add(_type, data, y); }
      Val sub(Val y) const override { return types::sub(_type, data, y); }

      Type<T>& _type;
      T data;
    };

    template <typename T, typename...Args>
    Val(Type<T>& type, Args&&...args): imp(make_shared<TImp<T>>(type, T(forward<Args>(args)...))) {} 

    Val(const Val&) = default;

    Val(Val&&) = default;

    Val &operator =(const Val &) = default;
    
    template <typename T>
    const T& as(Type<T>& type) const {
      if (&get_type(*this) != &type) { throw runtime_error("Wrong type"); }
      return dynamic_cast<const TImp<T>&>(*imp).data;
    }
    
    shared_ptr<const Imp> imp;
  };

  inline Trait& get_type(const Val &val) { return val.imp->type(); }
  
  namespace types {
    template <typename T>
    PC call(Type<T>& type, const T& imp, Thread& thread, PC pc, Pos pos) { throw runtime_error("Not implemented"); }

    template <typename T>
    void dump(Type<T>& type, const T& imp, ostream& out) { throw runtime_error("Not implemented"); }

    template <typename T>
    void say(Type<T>& type, const T& imp, ostream& out) { dump(type, imp, out); }

    template <typename T>
    bool is_true(Type<T> &type, const T& x) { return true; }

    template <typename T>
    bool eq(Type<T> &type, const T& x, Val y) { return x == y.as(type); }

    template <typename T>
    bool gt(Type<T> &type, const T& x, Val y) { return x > y.as(type); }

    template <typename T>
    bool lt(Type<T> &type, const T& x, Val y) { return x < y.as(type); }

    template <typename T>
    Val add(Type<T> &type, const T& x, Val y) { throw runtime_error("Not implemented"); }

    template <typename T>
    Val sub(Type<T> &type, const T& x, Val y) { throw runtime_error("Not implemented"); }
  }

  inline PC call(const Val& x, Thread& thread, PC pc, Pos pos) { return x.imp->call(thread, pc, pos); }
  
  constexpr bool operator==(const Val& x, const Val& y) { return x.imp->eq(y); }

  constexpr bool operator>(const Val& x, const Val& y) { return x.imp->gt(y); }

  constexpr bool operator<(const Val& x, const Val& y) { return x.imp->lt(y); }

  inline Val operator+(const Val& x, const Val& y) { return x.imp->add(y); }

  inline Val operator-(const Val& x, const Val& y) { return x.imp->sub(y); }

  inline void dump(const Val &v, ostream &out) { v.imp->dump(out); }

  inline void say(const Val &v, ostream &out) { v.imp->say(out); }
  
  inline ostream &operator<<(ostream &out, const Val &v) {
    say(v, out);
    return out;
  }

  inline bool is_true(const Val& x) { return x.imp->is_true(); }
}

#endif
