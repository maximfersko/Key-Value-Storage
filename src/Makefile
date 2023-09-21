ALL =  $(foreach dir, $(DIR), $(wildcard $(dir)/*.cc  $(dir)/*.h))
CC  =  $(foreach dir, $(DIR), $(wildcard $(dir)/*.cc))
DIR = ../controller ../data . 
CPPCHECKFLAGS    =  --enable=all --language=c++ \
                	--std=c++17 --suppress=missingIncludeSystem \
					--suppress=unusedFunction --suppress=missingInclude \
					--suppress=unusedStructMember --suppress=unmatchedSuppression \
					--enable=warning,style,performance,portability \
					--enable=all --inconclusive
CFLAGS = -Werror -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -g


lint:
	@clang-format -i --verbose $(ALL) tests/*.cc

build:
	g++ -std=c++17 $(CFLAGS) $(CC) -o main
	./main

test:
	g++ -std=c++17 $(CC) -o main
	./main

cppcheck:
	cppcheck $(CPPCHECKFLAGS) $(ALL)

tests: clean
	g++ -std=c++17  tests/*.cc $(CC) -lgtest -o test
	./test

clean:
	rm -rf main