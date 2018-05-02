#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>

void test_parser (const std::string &name)
{
    Scaner scaner(name);
    Lex_seq seq(scaner);
    Parser parser(seq);
    try {
        parser.parse();
        parser.print_poliz();
        parser.print_expr();
    } catch (const Exception &excp) {
        std::cout << excp << std::endl;
    } 
}

int main() {
  try {
    Scanner scan("Tests/lexer0.txt");
    Lex_seq a(scan);
    for (Lex b : a) {
      std::cout << b << ' ' ;
    }
    std::cout << std::endl;
  } catch (Exception e) {
    std::cout << e << std::endl;
  }
  test_parser("tests/lexer0.txt");
  return 0;
}
