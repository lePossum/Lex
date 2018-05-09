#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
  Scanner sc(argv[1]);
  Lexem_sequence seq(sc);
  Parser pars(seq);
  try {
    pars.parse();
    //pars.print_poliz();
    pars.print_expression();
  } catch (const Exception &excp) {
    std::cout << excp << std::endl;
  } 
  return 0;
}
