#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <cctype>

#include "lexer.hpp"

Lex Scanner::get_lex () 
{
  do {
    if (read_next) {
      c = file.get();
      ++char_num;
    } else {
      read_next = true;
    }
    switch (state) {
      case S :
        if (c == ' ' || c == '\n' || 
          c == '\t' || c == '\r' ) {
          if (c == '\n') {
            ++str_num;
            char_num = 0;
          }
          break;
        } else {
          buf.clear();
          if (isalpha(c)) {
            state = IDENT;
            buf += c;
            break;
          }
          if (isdigit(c)) {
            state = NUM;
            buf += c;
            break;
          }    
          if (c == EOF) {
            return Lex (LEX_FIN);
          }
          state = SIGN;
          read_next = false;
        }
        break;
      case IDENT :
        if (isalpha(c) || isdigit(c)) {
          buf += c;
        } else {
          state = S;
          read_next = false;
          return Lex (LEX_ID, buf);
        }
        break;
      case NUM :
        if (isdigit(c)) {
          buf += c;
        } else {
          state = S;
          read_next = false;
          return Lex (LEX_NUM, buf);
        }
        break;
      case SIGN :
        switch (c) {
        case '+' :
          state = S; 
          return Lex (LEX_PLUS, " + ");
        case '*' : 
          state = S; 
          return Lex (LEX_TIMES, " * ");
        case '(' : 
          state = S; 
          return Lex (LEX_LPAREN, "(");
        case ',' :
          state = S;
          return Lex (LEX_COMMA, ", ");
        case ')' : 
          state = S; 
          return Lex (LEX_RPAREN, ")");
        default : 
          throw Exception("Wrong character", str_num, 
            char_num, TYPE_LEX);
        }
    }
  } while (true);
}            

Lex Lex_seq_iter::operator* () 
{
  if (lex.get_lex_type() == LEX_FIN) {
    lex_num = -1;
  }
  return lex;
}

Lex_seq_iter& Lex_seq_iter::operator++ () 
{
  if (lex_num != -1) {
    ++lex_num;
  }
  lex = scanner->get_lex();
  return *this;
}

std::ostream& operator<<(std::ostream& str, Lex lex) 
{
  switch (lex.lex_type) {
    case LEX_ID :
      str << lex.name;
      break;
    case LEX_NUM :
      str << lex.name;
      break;
    case LEX_PLUS :
      str << "+";
      break;
    case LEX_TIMES :
      str << "*";
      break;
    case LEX_LPAREN :
      str << "(";
      break;
    case LEX_COMMA :
      str << ',';
      break;
    case LEX_RPAREN :
      str << ")";
      break;
    default :
      str << lex.lex_type << ", " << lex.name;
    }
  return str;
}

std::ostream& operator<<(std::ostream& str, Type type_in)
{
  if (type_in.if_func != 0) {
    str << "function that returns ";
  }
  switch (type_in.type) {
    case TYPE_INT :
      str << "integer";
      return str;
    case TYPE_STRING :
      str << "string";
      return str;
  default :
    str << type_in.type;
    return str;
  }
}
      
std::ostream& operator<<(std::ostream& str, Exception cur_exp)
{
  switch (cur_exp.type) {
    case TYPE_LEX :
      str << "Lexical error, ";
      break;
    case TYPE_SYN :
      str << "Syntactic error, ";
      break;
    case TYPE_SEM :
      str << "Semantic error, ";
      break;
    default:
      str << "Unknown type of errors, ";
      break;
  }
  str << cur_exp.str_num << ":" 
    << cur_exp.char_num << ": " << cur_exp.reason;
  return str;
}
