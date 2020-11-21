#ifndef LGPP_VM_HPP
#define LGPP_VM_HPP

#include <vector>

#include "op.hpp"
#include "stack.hpp"

namespace lgpp {

  using namespace std;

  struct VM {
    template <typename T, typename...Args>
    void emit(Args&&...args) {
      ops.emplace_back(ops.size(), T(forward<Args>(args)...));
    }

    const Op &eval(PC start_pc, Stack &stack);
    const Op &eval(const Op &start_op, Stack &stack);

    vector<Op> ops;
  };

}

#endif
