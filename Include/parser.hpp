#ifndef PARSER
#define PARSER

#include "lexer.hpp"
#include <stack>
#include <vector>

class Parser {   
  Lex_seq_iter cur_pos;
  Lex cur_lex;
  type_of_lex cur_type;
  std::stack<Type> st_type;
  std::vector<Lex> poliz;
  void get_lex ();
  void S ();
  void Expr ();
  void Plus_expr ();
  void Plus_expr0 ();
  void Mult_expr ();
  void Mult_expr0 ();
  void Func ();
  void check ();
  void check_op ();
public :
  Parser (Lex_seq &lex_seq) : cur_pos(lex_seq.begin()) {}
  void parse ();
  void print_poliz ();
  void print_expression ();
  void print_stack();
};

#endif
