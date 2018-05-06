LIGHT_FLAGS = -g -std=gnu++14 -I Include
HEAVY_FLAGS = -g -std=gnu++14 -O2 -Wall -Werror -pedantic-errors -Wextra -fsanitize=undefined
OBJECTS = Source/lexer.cpp Source/main.cpp Source/parser.cpp

debug :
	g++ $(LIGHT_FLAGS) $(OBJECTS) -o a.out
full :
	g++ $(HEAVY_FLAGS) $(OBJECTS) -o b.out
clean :
	rm -rf *.out
	rm -rf Coverage	
	rm -rf Results
	rm -rf usr
	rm -rf 1.gcda
	rm -rf 1.gcno
coverage :
	g++ $(LIGHT_FLAGS) --coverage $(OBJECTS) -o cov.out
	./cov.out
gcover : coverage
	gcov main.cpp -b
	mkdir -p Coverage
	mv *.gcda *.gcno *.gcov Coverage
lcoverage : gcover
	lcov/bin/lcov --capture --directory Coverage --rc lcov_branch_coverage=1 --output-file Coverage/coverage.info
	lcov/bin/genhtml Coverage/coverage.info --branch-coverage
	mv *.html Coverage
	mv *.png Coverage
	mv *.css Coverage
	cp -r Source Coverage
	rm -f Source/*.html
	rm -f Source/*.png
	rm -f Source/*.css
	rm -f Source/*.info
	cp -r Include Coverage
	rm -f Include/*.html
	rm -f Include/*.png
	rm -f Include/*.css
	rm -f Include/*.info
	cp -r 5 Coverage
	cp -r home Coverage
	rm -rf 5
	rm -rf home
download : 
	git clone https://github.com/linux-test-project/lcov.git
