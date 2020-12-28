#ifndef LGPP_ERROR_HPP
#define LGPP_ERROR_HPP

#include <string>
#include <sstream>

#include "lgpp/pos.hpp"

namespace lgpp {

  using namespace std;
  
  struct Error: runtime_error {
    template <typename...Args>
    static string format_msg(lgpp::Pos pos, Args&&...args) {
      stringstream buf;
      buf << "Error in '" << pos.file << "' at row " << pos.row << ", col " << pos.col << ':' << endl;
      (buf << ... << args);
      return buf.str();
    }

    template <typename...Args>
    Error(Pos pos, Args&&...args): runtime_error(format_msg(pos, forward<Args>(args)...)) {}
  };

  struct EParse: Error {
    template <typename...Args>
    EParse(Args&&...args): Error(forward<Args>(args)...) {}
  };

  struct ECompile: Error {
    template <typename...Args>
    ECompile(Args&&...args): Error(forward<Args>(args)...) {}
  };

  struct ERun: Error {
    template <typename...Args>
    ERun(Args&&...args): Error(forward<Args>(args)...) {}
  };

}

#endif
