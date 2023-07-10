CFLAGS = -Wall -Werror -Wextra

all: s21_matrix_oop.a

s21_matrix_oop.a:
	gcc -c $(CFLAGS) -lstdc++ -std=c++17 s21_matrix_oop.cc -o s21_matrix_oop.o -pedantic
	ar rcs s21_matrix_oop.a s21_matrix_oop.o
	ranlib s21_matrix_oop.a

clean:
	rm -rf *.o s21_matrix_oop.a test
	rm -rf *.gcda *.gcno report gcov_report.* gcov_report *.info

leaks: s21_matrix_oop.a
	gcc -lgtest -lstdc++ -std=c++17 s21_matrix_oop.a unit_tests.cc -o test -fsanitize=address -pedantic
	leaks -atExit -- ./test

clang:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=google -n *.cc *.h
	rm .clang-format

test: s21_matrix_oop.a
	gcc -lgtest -lstdc++ -std=c++17 s21_matrix_oop.a unit_tests.cc -o test -pedantic
	./test

gcov_report: clean s21_matrix_oop.a
	gcc  --coverage -lgtest -lstdc++ -std=c++17 s21_matrix_oop.cc unit_tests.cc -o gcov_report -pedantic
	./gcov_report
	rm unit_tests.gcda
	lcov -t "stest" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html

install_lcov:
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

.PHONY: all clang s21_matrix_oop.a test install_lcov leaks gcov_report clean