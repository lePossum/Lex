#ifndef LEXER
#define LEXER

#include <iostream>
#include <fstream>
#include <vector>

enum type_of_lex {
  LEX_NULL,    //0
  LEX_ID,      //1
  LEX_NUM,     //2
  LEX_PLUS,    //3
  LEX_TIMES,   //4
  LEX_LPAREN,  //5
  LEX_COMMA,   //6
  LEX_RPAREN,  //7
  LEX_FIN      //8
};

enum excp_types {
  TYPE_LEX, 
  TYPE_SYN, 
  TYPE_SEM
};

enum types {
  TYPE_NULL,
  TYPE_INT,
  TYPE_STRING,
  TYPE_FUNC
};

class Type {
  public :
  types type;
  int if_func;
  Type (types type = TYPE_NULL, int if_func = 0) 
    : type(type), if_func(if_func) {}
  bool operator==(const Type &type) { 
    return (this->type == type.type && 
      this->if_func == type.if_func); }
  bool operator!=(const Type &type) { 
    return (this->type != type.type || 
      this->if_func != type.if_func); }
};

std::ostream& operator<<(std::ostream&, Type);

class Lex {
  type_of_lex lex_type;
  std::string name;
  Type type;
public:
  Lex (type_of_lex lex_type = LEX_NULL, 
    const std::string &name = "", const Type &type = Type()) 
    : lex_type(lex_type), name(name), type(type)  {}
  type_of_lex get_lex_type() const { return lex_type; }
  std::string get_name() const { return name; }
  Type get_type() const { return type; }
  void set_type(Type new_type) { type = new_type; }
  friend std::ostream& operator << (std::ostream& str, Lex lex);
}; 

class Scanner {

  enum states {S, IDENT, NUM, SIGN};
  std::ifstream file;
  states state;
  std::string buf;
  bool read_next;
  char c;
public :
  int str_num;
  int char_num;
  Scanner (const std::string& name) : 
    state(S), read_next(true), str_num(1), 
    char_num(0) { file.open(name); }
  Lex get_lex();

};  

class Lexem_iterator : public std::iterator 
  <std::input_iterator_tag, std::string> {
  Scanner *scanner;
  int lex_num;
  Lex lex;
public :
  int get_str_num() { return scanner->str_num; }
  int get_char_num() { return scanner->char_num; }
  //int char_num;
  Lexem_iterator (Scanner *scanner, int lex_num) 
    : scanner(scanner), lex_num(lex_num) {};
  Lex operator*(); 
  Lexem_iterator& operator++();
  Lexem_iterator operator++(int);
  bool operator==(Lexem_iterator iter) const {
    return (lex_num == iter.lex_num);
  }
  bool operator!=(Lexem_iterator iter) const {
    return (lex_num != iter.lex_num);
  }
};

class Lexem_sequence {
  Scanner *scanner;
  public :
  Lexem_sequence(Scanner &scanner) : scanner(&scanner) {}
  Lexem_iterator begin() {
    return Lexem_iterator(scanner, 0);
  }
  Lexem_iterator end() {
    return Lexem_iterator(scanner, -1);
  }
  Lexem_iterator begin() const {
    return Lexem_iterator(scanner, 0);
  }
  Lexem_iterator end() const {
    return Lexem_iterator(scanner, -1);
  }
};

struct Exception {
public:
  std::string reason;
  int str_num;
  int char_num;
  excp_types type;
  Exception(const std::string &reason, 
    const int str_num, const int char_num, 
    const excp_types type) 
    : reason(reason), str_num(str_num), 
    char_num(char_num), type(type) {}
};

std::ostream& operator<<(std::ostream&, Exception);

#endif
