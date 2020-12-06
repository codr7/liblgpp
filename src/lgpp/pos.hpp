#ifndef LGPP_POS_HPP
#define LGPP_POS_HPP

#include <string>

namespace lgpp {
  using namespace std;
  
  struct Pos {
    static const int START_ROW = 1, START_COL = 0;
    
    Pos(string file, int row=START_ROW, int col=START_COL): file(file), row(row), col(col) {}

    string file;
    int row, col;
  };
}

#endif
