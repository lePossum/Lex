#include "parser.hpp"

void Parser::get_lex () 
{
  ++cur_pos;
  cur_lex = *cur_pos;
  cur_type = cur_lex.get_lex_type();
}   

void Parser::parse ()
{
  get_lex();
  S();
}

void Parser::S ()
{
  while (cur_type != LEX_FIN) {
  Expr();
  }
}

void Parser::Expr ()
{
  Plus_expr();
  Plus_expr0();
}

void Parser::Plus_expr0 ()
{
  if (cur_type == LEX_PLUS) {
  get_lex();
  Plus_expr();
  check_op();
  poliz.push_back(Lex(LEX_PLUS));
  Plus_expr0();
  }
}

void Parser::Plus_expr ()
{
  Mult_expr();
  Mult_expr0();
}

void Parser::Mult_expr0 ()
{
  if (cur_type == LEX_TIMES) {
  get_lex();
  Mult_expr();
  check_op();
  poliz.push_back(Lex(LEX_TIMES));
  Mult_expr0();
  }
}

void Parser::Mult_expr ()
{
  switch (cur_type) {
  case LEX_ID : 
    check();
    st_type.push(cur_lex.get_type());
    poliz.push_back(cur_lex);
    get_lex();
    Func();
    break;
  case LEX_NUM : 
    cur_lex.set_type(Type(TYPE_INT, 0));
    st_type.push(Type(TYPE_INT, 0));
    poliz.push_back(cur_lex);
    get_lex();
    break;
  default :
    throw Exception("Wrong lexem", cur_pos.get_str_num(),
     cur_pos.get_char_num(), TYPE_SYN);
  }
}

void Parser::Func ()
{
  if (cur_type == LEX_LPAREN) {
    std::string func_name = poliz[poliz.size() - 1].get_name();
    poliz.push_back(cur_lex);
    get_lex();
    for (int i = 2; i < func_name.size(); i++){
      if (func_name[i] == 'i' || func_name[i] == 'j' || func_name[i] == 'k'){
        st_type.push(Type(TYPE_INT, 0));
        Expr();
        check_op();
      } else if (func_name[i] == 's' || func_name[i] == 't') {
        st_type.push(Type(TYPE_STRING, 0));
        Expr();
        check_op();
        if (i > 1) st_type.pop();
      } else if (func_name[i] == 'f') {
        std::cout << "Bad recursive function" << std::endl;
        throw Exception("Wrong function identifier",cur_pos.get_str_num(), 
          cur_pos.get_char_num(), TYPE_SYN);
      }
      if (cur_type != LEX_COMMA && cur_type != LEX_RPAREN) {
        throw Exception("Wrong factual elements of function",
          cur_pos.get_str_num(), cur_pos.get_char_num(), TYPE_SYN);
      }
      poliz.push_back(cur_lex);
      get_lex();
    }
  }
}

void Parser::check ()
{
  std::string name = cur_lex.get_name();
  char letter = 0;
  int cnt = -1;
  bool func_flag = false;
  for (auto c : name) {
    if (c != 'i' && c != 'j' && c != 'k' && c != 's' && c != 't' && c != 'a' && c != 'f') {
      throw Exception("Wrong identifier", cur_pos.get_str_num(), 
        cur_pos.get_char_num(), TYPE_LEX);
    } else {
      if (c == 'f') {
        func_flag = true;
      } else {
        ++cnt;
        letter = c;
        if (func_flag) break;
      }
    }
  }
  if (letter == 'i' || letter == 'j' || letter == 'k') {
    cur_lex.set_type(Type(TYPE_INT, cnt));
  } else if (letter == 's' || letter == 't') { 
    cur_lex.set_type(Type(TYPE_STRING, cnt));
  } else if (letter == 'f') {
    check();
  } else { 
    throw Exception("Wrong identifier", cur_pos.get_str_num(), 
      cur_pos.get_char_num(), TYPE_LEX);
  }
}


void Parser::check_op ()
{
  Type type1, type2;
  type2 = st_type.top();
  st_type.pop();
  type1 = st_type.top();
  st_type.pop();
  if (type1 == type2) {
      st_type.push(type1);
  } else {
    throw Exception("Wrong types", cur_pos.get_str_num(), 
      cur_pos.get_char_num(), TYPE_SEM);
  }
}

void Parser::print_poliz ()
{
  std::cout << "Poliz: ";
  for (const Lex &l : poliz) {
    std::cout << l << ' ';
  }
  std::cout << std::endl << std::endl;
}

void Parser::print_expression ()
{
  std::stack<Lex> st_poliz;
  Lex lex0;
  int i = 1;
  for (Lex current_lexem : poliz) {
    type_of_lex lex_type = current_lexem.get_lex_type();
    if (lex_type != LEX_PLUS && lex_type != 
      LEX_TIMES && lex_type != LEX_RPAREN) {
      if (lex_type == LEX_LPAREN) {
        Lex lex1 = st_poliz.top();
        st_poliz.pop();
        Type type = lex1.get_type();
        --type.if_func;
        lex1.set_type(type);
        st_poliz.push(lex1);
      }
      st_poliz.push(current_lexem);
    } else {
      if (lex_type == LEX_RPAREN) {
        do {
          lex0 = st_poliz.top();
          st_poliz.pop();
        } while (lex0.get_lex_type() != LEX_LPAREN);
      } else {
        Lex lex1 = st_poliz.top();
        st_poliz.pop();
        Lex lex2 = st_poliz.top();
        st_poliz.pop();
        std::cout << ' ' << i << ") " << lex2 
          << ' ' << current_lexem << ' ' << lex1 << " - " 
          << Type(TYPE_INT, 0) << std::endl;
        std::string temp_lex = (current_lexem.get_lex_type() 
          == LEX_PLUS ? " + " : " * ");
        std::string temp_name = lex2.get_name() + temp_lex;
        st_poliz.push(Lex(lex1.get_lex_type(), 
          temp_name + lex1.get_name(),
          lex1.get_type()));
        ++i;
      }
    }
  }
  std::cout << std::endl << "Subexpressions: " << std::endl;
  while (!st_poliz.empty()) {
    Lex lex = st_poliz.top();
    std::cout << ' ' << i << ") " << st_poliz.top() << " - " << lex.get_type() << std::endl;
    ++i;
    st_poliz.pop();
  }
}
