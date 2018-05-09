#include "parser.hpp"

void Parser::get_lex () 
{
  ++cur_pos;
  cur_lex = *cur_pos;
  //std::cout << cur_lex << std::endl;
  cur_type = cur_lex.get_lex_type();
}   

void Parser::parse ()
{
  get_lex();
  S();
  //std::cout << "*** S() done ***" << std::endl;
}

void Parser::S ()
{
  //std::cout << cur_type << std::endl;
  while (cur_type != LEX_FIN) {
  //std::cout << cur_lex << std::endl;
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
    //std::cout << "In Mult_expr() type of lex: " << cur_lex.get_type() << std::endl;
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
    throw Exception("Wrong lexem", cur_pos.get_str_num(), cur_pos.get_char_num());
  }
}

void Parser::Func ()
{
  //std::cout << cur_lex << std::endl;
  if (cur_type == LEX_LPAREN) {
    std::string func_name = poliz[poliz.size() - 1].get_name();
    //std::cout << "In Func() func_name is " << func_name << std::endl;
    poliz.push_back(cur_lex);
    get_lex();
    for (int i = 2; i < func_name.size(); i++){
      if (func_name[i] == 'i' || func_name[i] == 'j' || func_name[i] == 'k'){
        st_type.push(Type(TYPE_INT, 0));
        // std::cout << "In cycle cur_lex: " << cur_lex << cur_lex.get_type() << std::endl; 
        Expr();
        check_op();
        // std::cout << cur_lex << ' ' << cur_type << std::endl;
        // std::cout <<std::endl << std::endl;
        // for (auto n : poliz) {
        //   std::cout << n<< std::endl;
        // }
        // std::cout <<std::endl << std::endl;
      } else if (func_name[i] == 's' || func_name[i] == 't') {
        st_type.push(Type(TYPE_STRING, 0));
        Expr();
        check_op();
        if (i > 1) st_type.pop();
      } else if (func_name[i] == 'f') {
        std::cout << "Bad recursive function" << std::endl;
        throw Exception("Wrong function identifier",cur_pos.get_str_num(), cur_pos.get_char_num());
      }
      if (cur_type != LEX_COMMA && cur_type != LEX_RPAREN) {
        throw Exception("Wrong factual elements of function",cur_pos.get_str_num(), cur_pos.get_char_num());
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
    //std::cout << '|' << c << "|\n";
    if (c != 'i' && c != 'j' && c != 'k' && c != 's' && c != 't' && c != 'a' && c != 'f') {
      throw Exception("Wrong identifier", cur_pos.get_str_num(), cur_pos.get_char_num());
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
    throw Exception("Wrong identifier", cur_pos.get_str_num(), cur_pos.get_char_num());
  }
}


void Parser::check_op ()
{
  Type type1, type2;
  type2 = st_type.top();
  //print_poliz();
  st_type.pop();
  type1 = st_type.top();
  //std::cout << type1 << " " << type2 << std::endl;
  st_type.pop();
  //if (type1 == Type(TYPE_INT, 0) && type2 == Type(TYPE_INT, 0)) {
  if (type1 == type2) {
    //if (type1 == Type(TYPE_INT,0))
      st_type.push(type1);
  } else {
    print_stack();
    throw Exception("Wrong types", cur_pos.get_str_num(), cur_pos.get_char_num());
  }
}

void Parser::print_poliz ()
{
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
  for (Lex lex : poliz) {
    type_of_lex lex_type = lex.get_lex_type();
    if (lex_type != LEX_PLUS && lex_type != LEX_TIMES && lex_type != LEX_RPAREN) {
      if (lex_type == LEX_LPAREN) {
        Lex lex1 = st_poliz.top();
        st_poliz.pop();
        Type type = lex1.get_type();
        --type.arr_dim;
        lex1.set_type(type);
        st_poliz.push(lex1);
      }
      st_poliz.push(lex);
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
        std::cout << ' ' << i << ") " << lex1 << ' ' << lex << ' ' << lex2 << " - " << Type(TYPE_INT, 0) << std::endl;
        st_poliz.push(Lex(lex1.get_lex_type(), "Expression " + std::to_string(i), lex1.get_type()));
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

void Parser::print_stack () {
  std::cout << "STACK\n";
  int i = 0;
  while (!st_type.empty()) {
    auto lex = st_type.top();
    std::cout << ' ' << i << ") " << st_type.top() << std::endl;
    ++i;
    st_type.pop();
  }
}