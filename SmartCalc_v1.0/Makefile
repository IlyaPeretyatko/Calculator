UNAME:=$(shell uname -s)

ifeq ($(UNAME),Linux)
	TEST_FLAGS = -pthread -lcheck_pic -pthread -lrt -lm -lsubunit
endif
ifeq ($(UNAME),Darwin)
	TEST_FLAGS = $(shell pkg-config --cflags --libs check)
endif

CFLAGS = -Wall -Wextra -Werror -std=c11 -c

all: clean smartCalc.a test gcov_report

test: test.o smartCalc.a
	$(CC) test.o smartCalc.a -o test -L. $(TEST_FLAGS)
	rm test.o
	./test

test.o:
	gcc $(CFLAGS) test.c -o test.o

smartCalc.a: smartCalc.o
	ar rcs smartCalc.a smartCalc.o
	ranlib smartCalc.a
	rm smartCalc.o

smartCalc.o: smartCalc.c
	gcc $(CFLAGS) smartCalc.c -o smartCalc.o

install:
	cd smartCalc && qmake && make && make clean
	cp -r smartCalc smartCalc/smartCalc.app ~/Desktop
	rm -rf ~/Desktop/smartCalc/

uninstall:
	rm -rf smartCalc/smartCalc.app
	rm -rf smartCalc/Makefile 
	rm -rf ~/Desktop/smartCalc.app
	rm -rf ~/Desktop/smartCalc/

dist: install
	mkdir archiv
	cp -r *.c *.h Makefile smartCalc/ archiv
	tar -cvzf smartCalc.tar.gz archiv

dvi:
	open README.md

clean:
	rm -rf *.o *.a *.out *.info *.html *.gc* test gcov_report smartCalc report smartCalc.tar.gz archiv out

gcov_report:
	gcc -fprofile-arcs -ftest-coverage test.c smartCalc.c -o gcov_report $(TEST_FLAGS)
	./gcov_report
	lcov --no-external --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory out

ifeq ($(UNAME), Linux)
	xdg-open out/index.html
else
	open -a "Google Chrome" out/index.html
endif

clang:
	clang-format -n -style=google *.c *.h

clang_update:
	clang-format -i -style=google *.c *.h

leak:
ifeq ($(UNAME),Darwin)
	CK_FORK=no leaks -atExit -- ./test
else
	valgrind --leak-check=full -s --track-origins=yes ./test
endif

rebuild: all
