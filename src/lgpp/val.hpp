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

    Val(const Val &) = default;

    Val(Val &&) = default;

    Val &operator =(const Val &) = default;
    
    template <typename T>
    T as(Type<T> &type) const {
      auto &timp = dynamic_cast<const TImp<T> &>(*imp);
      if (&timp.type != &type) { throw runtime_error("Wrong type"); }
      return timp.data;
    }
    
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
    Val add(Type<T> &type, const T &x, Val y) { throw runtime_error("Not implemented"); }

    template <typename T>
    Val sub(Type<T> &type, const T &x, Val y) { throw runtime_error("Not implemented"); }
  }

  constexpr bool operator==(const Val &x, const Val &y) { return x.imp->eq(y); }

  constexpr bool operator>(const Val &x, const Val &y) { return x.imp->gt(y); }

  constexpr bool operator<(const Val &x, const Val &y) { return x.imp->lt(y); }

  inline Val operator+(const Val &x, const Val &y) { return x.imp->add(y); }

  inline Val operator-(const Val &x, const Val &y) { return x.imp->sub(y); }
  
}

#endif
