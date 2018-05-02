#include "lexer.hpp"
#include <iostream>

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
  return 0;
}
