LIGHT_FLAGS = -g -std=gnu++14 -I Include
HEAVY_FLAGS = -g -std=gnu++14 -O2 -Wall -Werror -pedantic-errors -Wextra -fsanitize=undefined
OBJECTS = Source/lexer.cpp Source/main.cpp Source/parser.cpp

debug :
	g++ $(LIGHT_FLAGS) $(OBJECTS) -o a.out
full :
	g++ $(HEAVY_FLAGS) $(OBJECTS) -o b.out
clean :
	rm -rf *.out
