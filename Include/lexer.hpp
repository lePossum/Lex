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
  LEX_RPAREN,  //6
  LEX_FIN      //7
};

enum types {
  TYPE_NULL,
  TYPE_INT,
  TYPE_STRING
};

class Type {
  public :
  types type;
  int arr_dim;
  Type (types type = TYPE_NULL, int arr_dim = 0) 
    : type(type), arr_dim(arr_dim) {}
  bool operator == (const Type &type) { 
    return (this->type == type.type && 
      this->arr_dim == type.arr_dim); }
  bool operator != (const Type &type) { 
    return (this->type != type.type || 
      this->arr_dim != type.arr_dim); }
};

std::ostream& operator << (std::ostream&, Type);

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
  Lex get_lex ();

};  

class Lex_seq_iter : 
  public std::iterator<std::input_iterator_tag, std::string> {
  
  Scanner *scanner;
  int lex_num;
  Lex lex;
public :
  int get_str_num () { return scanner->str_num; }
  int get_char_num () { return scanner->char_num; }
  int char_num;
  Lex_seq_iter (Scanner *scanner, int lex_num) 
    : scanner(scanner), lex_num(lex_num) {};
  Lex operator* (); 
  Lex_seq_iter& operator++ ();
  Lex_seq_iter operator++ (int);
  bool operator== (Lex_seq_iter iter) const {
    return (lex_num == iter.lex_num);
  }
  bool operator!= (Lex_seq_iter iter) const {
    return (lex_num != iter.lex_num);
  }
  
};

class Lex_seq {
  Scanner *scanner;
  public :
  Lex_seq (Scanner &scanner) : scanner(&scanner) {}
  Lex_seq_iter begin () {
    return Lex_seq_iter(scanner, 0);
  }
  Lex_seq_iter end () {
    return Lex_seq_iter(scanner, -1);
  }
  Lex_seq_iter begin () const {
    return Lex_seq_iter(scanner, 0);
  }
  Lex_seq_iter end () const {
    return Lex_seq_iter(scanner, -1);
  }
};

class Exception {
   
  public :
  std::string reason;
  int str_num;
  int char_num;
  Exception (const std::string &reason, 
    const int str_num, const int char_num) 
    : reason(reason), str_num(str_num), 
    char_num(char_num) {}
};

std::ostream& operator<<(std::ostream&, Exception);

#endif
