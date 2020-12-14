#ifndef LGPP_PARSER_HPP
#define LGPP_PARSER_HPP

#include <deque>
#include <optional>
#include <sstream>

#include "lgpp/error.hpp"
#include "lgpp/tok.hpp"
#include "lgpp/toks.hpp"
#include "lgpp/types.hpp"

namespace lgpp {
  using namespace std;

  struct Parser;
  
  size_t parse_id(Parser&, char, istream&);
  size_t parse_int(Parser&, char, istream&);

  struct Parser {
    using Alt = function<size_t (Parser&, char, istream&)>;
    
    Parser(string file): pos(move(file)) {
      alts.push_back(&parse_int);
      alts.push_back(&parse_id);
    }
    
    Pos pos;
    deque<Tok> toks;
    deque<Alt> alts;
  };

  template <typename T, typename...Args>
  const Tok& push(Parser& parser, Pos pos, Args&&...args) {
    return parser.toks.emplace_back(pos, T(forward<Args>(args)...));
  }
  
  inline optional<Tok> peek(const Parser& parser) {
    return parser.toks.empty() ? nullopt : make_optional(parser.toks.front());
  }

  inline Tok pop(Parser& parser) {
    Tok t = parser.toks.front();
    parser.toks.pop_front();
    return t;
  }

  inline size_t skip(Parser &parser, istream &in) {
    int n = 0;
    char c = 0;
    
    while (in.get(c) && isspace(c)) {
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
      };
    }
    
    if (!in.eof()) { in.unget(); }
    return n;
  }

  inline size_t parse_id(Parser& parser, char c, istream& in) {
    if (!isgraph(c)) { return 0; }
    auto p(parser.pos);
    stringstream buf;

    for (;;) {  
      buf << c;
      parser.pos.col++;

      if (!in.get(c) || !isgraph(c)) {
        in.unget();
        break;
      }
    }

    if (!buf.tellp()) { return 0; }
    push<toks::Id>(parser, p, buf.str());
    return 1;
  }

  int parse_int(Parser &parser, char c, istream &in, int base) {
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
  
  inline size_t parse_int(Parser& parser, char c, istream& in) {
    if (!isdigit(c)) { return 0; }
    auto p = parser.pos;
    push<toks::Lit>(parser, p, types::Int, parse_int(parser, c, in, 10));
    return 1;
  }
  
  inline size_t parse_tok(Parser& parser, istream& in) {
    if (char c = 0; in.get(c)) {
      for (auto &a: parser.alts) {
	auto n = a(parser, c, in);
	if (n) { return n; }
      }

      throw EParse(parser.pos, "Unexpected input: '", c, "'");
    }

    return 0;
  }

  template <typename T = toks::Group>
  Parser::Alt parse_group(char beg, char end) {
    return [beg, end](Parser& parser, char c, istream& in) -> size_t {
      if (c != beg) { return 0; }
      Pos p = parser.pos;
      vector<Tok> toks;
      auto i = parser.toks.size();

      for(;;) {
	if (!in.get(c)) { throw EParse(parser.pos, "Missing end"); }
	if (c == end) { break; }
	in.unget();
	if (!parse_tok(parser, in)) { break; }
	skip(parser, in);
      }

      move(parser.toks.begin() + i, parser.toks.end(), back_inserter(toks));
      parser.toks.erase(parser.toks.begin() + i, parser.toks.end());
      push<T>(parser, p, toks);
      return 1;
    };
  }

  inline size_t parse(Parser& parser, string in) {
    size_t n = 0;
    istringstream is(in);
    
    for (;;) {
      skip(parser, is);
      auto pn = parse_tok(parser, is);
      if (!pn) { break; }
      n += pn;
    }
    
    return n;
  }

  inline void compile(Parser& parser, Thread& out, Env& env) {
    while (!parser.toks.empty()) { compile(pop(parser), parser, out, env); }
  }
}

#endif
