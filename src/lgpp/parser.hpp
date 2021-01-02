#ifndef LGPP_PARSER_HPP
#define LGPP_PARSER_HPP

#include <optional>
#include <sstream>

#include "lgpp/error.hpp"
#include "lgpp/toks/group.hpp"
#include "lgpp/toks/id.hpp"
#include "lgpp/toks/lit.hpp"

namespace lgpp {
  using namespace std;

  struct Parser;
  
  bool parse_id(Parser&, char, istream&);
  bool parse_int(Parser&, char, istream&);

  struct Parser {
    using Alt = function<bool (Parser&, char, istream&)>;
    
    Parser(VM& vm, string file): vm(vm), pos(move(file)) {}

    VM& vm;
    Pos pos;
    Toque toks;
    deque<Alt> alts;
  };

  template <typename T, typename...Args>
  const Tok& push(Parser& parser, Pos pos, Args&&...args) { return push<T>(parser.toks, pos, forward<Args>(args)...); }
  
  inline optional<Tok> peek(const Parser& parser) { return peek(parser.toks); }

  inline Tok pop(Parser& parser) { return pop(parser.toks); }

  inline Tok pop_back(Parser& parser) { return pop_back(parser.toks); }

  inline size_t skip(Parser &parser, istream &in) {
    size_t n = 0;
    char c = 0;
    
    while (in.get(c)) {
      if (!isspace(c)) {
	in.unget();
	break;
      }
	  
      switch (c) {
      case ' ':
      case '\t':
	parser.pos.col++;
	n++;
        break;
      case '\n':
        parser.pos.row++;
        parser.pos.col = Pos::START_COL;
	n++;
	break;
      };
    }
    
    return n;
  }
  
  inline bool parse_id_pred(Parser& parser, char c, istream& in, function<bool (char c)> pred) {
    if (!isgraph(c)) { return false; }
    auto p(parser.pos);
    stringstream buf;

    for (;;) {  
      buf << c;
      parser.pos.col++;

      if (!in.get(c) || !isgraph(c) || (pred && !pred(c))) {
        in.unget();
        break;
      }
    }

    if (!buf.tellp()) { return false; }
    push<toks::Id>(parser, p, buf.str());
    return true;
  }

  inline bool parse_id(Parser& parser, char c, istream& in) {
    return parse_id_pred(parser, c, in, nullptr);
  }

  int parse_int_base(Parser &parser, char c, istream &in, int base) {
    int v(0);
    
    static map<char, int8_t> char_vals = {
      {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
      {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14},
      {'f', 15}
    };
    
    auto ci(char_vals.end());
    
    do {
      if ((ci = char_vals.find(c)) == char_vals.end()) { break; }
      auto cv(ci->second);
      if (cv >= base) { throw EParse(parser.pos, "Invalid integer: ", c); }
      v = v * base + cv;
      parser.pos.col++;
    } while (in.get(c));
    
    if (!in.eof()) { in.unget();}
    return v;
  }
  
  inline bool parse_int(Parser& parser, char c, istream& in) {
    if (!isdigit(c)) { return false; }
    auto p = parser.pos;
    push<toks::Lit>(parser, p, parser.vm.Int, parse_int_base(parser, c, in, 10));
    return true;
  }
  
  inline bool parse_tok(Parser& parser, istream& in) {
    if (char c = 0; in.get(c)) {
      for (auto &a: parser.alts) {
	if (a(parser, c, in)) { return true; }
      }

      throw EParse(parser.pos, "Unexpected input: '", c, "'");
    }

    return false;
  }

  template <typename T = toks::Group>
  Parser::Alt parse_group(char beg, char end) {
    return [beg, end](Parser& parser, char c, istream& in) -> bool {
      if (c != beg) { return false; }
      Pos p = parser.pos;
      vector<Tok> toks;
      auto i = parser.toks.size();

      for(;;) {
	skip(parser, in);
	if (!in.get(c)) { throw EParse(parser.pos, "Missing end of expression: ", end); }
	if (c == end) { break; }
	in.unget();
	if (!parse_tok(parser, in)) { throw EParse(parser.pos, "Invalid token"); }
      }

      move(parser.toks.begin() + i, parser.toks.end(), back_inserter(toks));
      parser.toks.erase(parser.toks.begin() + i, parser.toks.end());
      push<T>(parser, p, toks);
      return true;
    };
  }

  inline void parse(Parser& parser, string in) {
    istringstream is(in);
    
    for (;;) {
      skip(parser, is);
      if (!parse_tok(parser, is)) { break; }
    }
  }

  inline void compile(Parser& parser, Thread& out, Env& env) {
    while (!parser.toks.empty()) { compile(pop(parser), parser.toks, out, env); }
  }
}

#endif
