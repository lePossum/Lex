#ifndef PARSER
#define PARSER

#include "lexer.hpp"
#include <stack>
#include <vector>

class Parser {   
  std::vector<Lex> poliz;
  std::stack<Type> st_type;
  Lexem_iterator cur_pos;
  Lex cur_lex;
  type_of_lex cur_type;
  
  void S();
  void Expr();
  void Plus_expr();
  void Plus_expr0();
  void Mult_expr();
  void Mult_expr0();
  void Func();

  void get_lex();
  void check_name();
  void check_types();
public :
  Parser(Lexem_sequence &lex_seq) : 
    cur_pos(lex_seq.begin()) {}
  void parse();
  void print_poliz();
  void print_expression();
};

#endif
