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
  std::cout << "*** S() done ***" << std::endl;
}

void Parser::S ()
{
  while (cur_type != LEX_FIN) {
    //std::cout << cur_lex << std::endl;
    Expr();
  }
}

void Parser::Expr ()
{
  //std::cout << cur_lex << std::endl;
  Plus_expr();
  //std::cout << cur_lex  << " RPAREN"<< std::endl;
  Plus_expr0();
}

void Parser::Plus_expr0 ()
{
  if (cur_type == LEX_PLUS) {
    get_lex();
    Plus_expr();
    //check_op();
    //poliz.push_back(Lex(LEX_PLUS));
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
    //check_op();
    //poliz.push_back(Lex(LEX_TIMES));
    Mult_expr0();
  }
}

void Parser::Mult_expr ()
{
  switch (cur_type) {
    case LEX_ID : 
      check();
      //st_type.push(cur_lex.get_type());
      //poliz.push_back(cur_lex);
      //std::cout << cur_lex.get_type() << std::endl;
      get_lex();
      //Id();
      break;
    case LEX_NUM : 
      //cur_lex.set_type(Type(TYPE_INT, 0));
      //st_type.push(Type(TYPE_INT, 0));
      //poliz.push_back(cur_lex);
      get_lex();
      break;
    case LEX_LPAREN :
      get_lex();
      //std::cout << cur_lex << std::endl;
      Expr();
      if (cur_type != LEX_RPAREN) {
        throw Exception("Wrong bracket expression", cur_pos.get_str_num(), cur_pos.get_char_num());
      }
      //std::cout << cur_lex << std::endl;
      get_lex();
      break;
    default :
      throw Exception("Wrong lexem", cur_pos.get_str_num(), cur_pos.get_char_num());
  }
}

void Parser::Func ()
{
  //std::cout << cur_lex << std::endl;
  
}

void Parser::check ()
{
  std::string name = cur_lex.get_name();
  char letter = 0;
  int cnt = -1;
  for (auto c : name) {
    //std::cout << '|' << c << "|\n";
    if (c != 'i' && c != 'j' && c != 'k' && c != 's' && c != 't' && c != 'a') {
      throw Exception("WRONG ID", cur_pos.get_str_num(), cur_pos.get_char_num());
    } else {
      ++cnt;
      letter = c;
    }
  }
  if (letter == 'i' || letter == 'j' || letter == 'k') {
    cur_lex.set_type(Type(TYPE_INT, cnt));
  } else {
    if (letter == 's' || letter == 't') {
        cur_lex.set_type(Type(TYPE_STRING, cnt));
    } else { 
        throw Exception("WRONG ID", cur_pos.get_str_num(), cur_pos.get_char_num());
    }
  }
}

void Parser::print_poliz ()
{
    for (const Lex &lex : poliz) {
        std::cout << lex << ' ';
    }
    std::cout << std::endl;
}

void Parser::print_expr ()
{
    std::stack<Lex> st_poliz;
    Lex lex0;
    int i = 1;
    for (Lex lex : poliz) {
    type_of_lex lex_type = lex.get_lex_type();
        if (lex_type != LEX_PLUS && lex_type != LEX_TIMES && lex_type != LEX_RSQPAR) {
            if (lex_type == LEX_LSQPAR) {
                Lex lex1 = st_poliz.top();
                st_poliz.pop();
                Type type = lex1.get_type();
                --type.arr_dim;
                lex1.set_type(type);
                st_poliz.push(lex1);
            }
            st_poliz.push(lex);
        } else {
            if (lex_type == LEX_RSQPAR) {
                do {
                    lex0 = st_poliz.top();
                    st_poliz.pop();
                } while (lex0.get_lex_type() != LEX_LSQPAR);
            } else {
                Lex lex1 = st_poliz.top();
                st_poliz.pop();
                Lex lex2 = st_poliz.top();
                st_poliz.pop();
                std::cout << i << ") " << lex1 << ' ' << lex << ' ' << lex2 << " -----> " << Type(TYPE_INT, 0) << std::endl;
                st_poliz.push(Lex(lex1.get_lex_type(), "Expr" + std::to_string(i), lex1.get_type()));
                ++i;
            }
        }
    }
    std::cout << "Standalone expresions : " << std::endl;
    while (!st_poliz.empty()) {
        Lex lex = st_poliz.top();
        std::cout << i << ") " << st_poliz.top() << " -----> " << lex.get_type() << std::endl;
        ++i;
        st_poliz.pop();
    }
}
