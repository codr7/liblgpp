#ifndef LGPP_VAL_HPP
#define LGPP_VAL_HPP

#include <memory>

namespace lgpp {  
  using namespace std;
  
  template <typename T>
  struct Type;

  struct Val;
  
  namespace types {
    
    template <typename T>
    bool eq(Type<T> &type, const T &x, Val y);

    template <typename T>
    bool gt(Type<T> &type, const T &x, Val y);

    template <typename T>
    bool lt(Type<T> &type, const T &x, Val y);

    template <typename T>
    Val add(Type<T> &type, const T &x, Val y);

    template <typename T>
    Val sub(Type<T> &type, const T &x, Val y);

  }

  struct Val {
    struct Imp {
      virtual ~Imp() = default;
      
      virtual bool eq(Val y) const = 0;
      virtual bool gt(Val y) const = 0;
      virtual bool lt(Val y) const = 0;

      virtual Val add(Val y) const = 0;
      virtual Val sub(Val y) const = 0;
    };

    template <typename T>
    struct TImp: Imp {
      TImp(Type<T> &type, T data): type(type), data(move(data)) {}

      bool eq(Val y) const override { return types::eq(type, data, y); }
      bool gt(Val y) const override { return types::gt(type, data, y); }
      bool lt(Val y) const override { return types::lt(type, data, y); }

      Val add(Val y) const override { return types::add(type, data, y); }
      Val sub(Val y) const override { return types::sub(type, data, y); }

      Type<T> &type;
      T data;
    };

    template <typename T>
    Val(Type<T> &type, T imp): imp(make_shared<TImp<T>>(type, move(imp))) {} 

    template <typename T>
    T as(Type<T> &type) const {
      auto &timp = dynamic_cast<const TImp<T> &>(*imp);
      if (&timp.type != &type) { throw runtime_error("Wrong type"); }
      return timp.data;
    }
    
    bool eq(Val y) const { return imp->eq(y); }
    bool gt(Val y) const { return imp->gt(y); }
    bool lt(Val y) const { return imp->lt(y); }

    Val add(Val y) const { return imp->add(y); }
    Val sub(Val y) const { return imp->sub(y); }

    shared_ptr<const Imp> imp;
  };

  namespace types {
    template <typename T>
    bool eq(Type<T> &type, const T &x, Val y) { return x == y.as(type); }

    template <typename T>
    bool gt(Type<T> &type, const T &x, Val y) { return x > y.as(type); }

    template <typename T>
    bool lt(Type<T> &type, const T &x, Val y) { return x < y.as(type); }

    template <typename T>
    Val add(Type<T> &type, const T &x, Val y) { return Val(type, x + y.as(type)); }

    template <typename T>
    Val sub(Type<T> &type, const T &x, Val y) { return Val(type, x - y.as(type)); }
  }

  constexpr bool operator==(const Val &x, const Val &y) { return x.eq(y); }

  constexpr bool operator>(const Val &x, const Val &y) { return x.gt(y); }

  constexpr bool operator<(const Val &x, const Val &y) { return x.lt(y); }

  inline Val operator+(const Val &x, const Val &y) { return x.add(y); }

  inline Val operator-(const Val &x, const Val &y) { return x.sub(y); }
  
}

#endif
