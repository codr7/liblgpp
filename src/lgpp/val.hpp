#ifndef LGPP_VAL_HPP
#define LGPP_VAL_HPP

namespace lgpp {  
  using namespace std;
  
  template <typename T>
  struct Type;

  struct Val;
  
  namespace types {
    template <typename T>
    bool eq(Type<T> &type, const T &x, const Val &y);
  }

  struct Val {
    struct Imp {
      virtual ~Imp() = default;
      virtual bool eq(const Val &y) const = 0;
    };

    template <typename T>
    struct TImp: Imp {
      TImp(Type<T> &type, T data): type(type), data(move(data)) {}

      bool eq(const Val &y) const override { return types::eq(type, data, y); }

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
    
    bool eq(const Val &y) const { return imp->eq(y); }

    shared_ptr<const Imp> imp;
  };
}

#endif
