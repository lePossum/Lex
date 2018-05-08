#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"

void test_parser (const std::string &name)
{
    Scanner scanner(name);
    Lex_seq seq(scanner);
    Parser parser(seq);
    try {
        parser.parse();
        parser.print_poliz();
        parser.print_expression();
    } catch (const Exception &excp) {
        std::cout << excp << std::endl;
    } 
}

int main(int argc, char* argv[]) {
  try {
    Scanner scan(argv[1]);
    Lex_seq a(scan);
    for (Lex b : a) {
      std::cout << b << ' ' ;
    }
    std::cout << std::endl;
  } catch (Exception e) {
    std::cout << e << std::endl;
  }
  test_parser(argv[1]);
  return 0;
}
