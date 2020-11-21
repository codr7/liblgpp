#include "stop.hpp"

namespace lgpp::ops {  
  template <>
  const Op *eval(const Op &op, const Stop &imp, lgpp::Stack &stack) {
    return nullptr;
  }
}
