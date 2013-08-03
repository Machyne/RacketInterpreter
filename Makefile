main: interpreter parser tokenizer
	make interpreter
	make parser
	make tokenizer

interpreter: interpreter.o primitives.o interpreter_main.c escape.o common.o parstack.o vector.o tokenizer.c
	clang -g interpreter_main.c interpreter.o primitives.o parstack.o tokenizer.c vector.o escape.o common.o -lm -o interpreter

primitives.o: primitives.c primitives.h interpreter.o common.o escape.o
	clang -g -c primitives.c

escape.o: escape.c escape.h
	clang -g -c escape.c

interpreter.o: interpreter.c interpreter.h common.o
	clang -g -c interpreter.c

vector.o: vector.c vector.h
	clang -g -c vector.c

parser: parser_main.c parstack.o tokenizer.c common.o
	clang -g parser_main.c parstack.o tokenizer.c common.o -lm -o parser

parstack.o: parstack.c parstack.h common.o
	clang -g -c parstack.c

tokenizer: tokenizer.c tokenizer_main.c common.o
	clang -g tokenizer.c common.o tokenizer_main.c -o tokenizer

common.o: common.c common.h
	clang -g -c common.c

tokenizer.c: tokenizer.l common.o
	flex -o tokenizer.c tokenizer.l

clean:
	rm -rf tokenizer.c *.o *.dSYM tokenizer parser interpreter *.*~ ._* *.orig testFiles/._* testFiles/*.*~

both:
	make clean
	make

vg:
	make
	valgrind --dsymutil=yes --leak-check=full ./interpreter < ./testfile.lisp

test:
	cat ./testFiles/tokenizer-test.input.01 | ./tokenizer | diff ./testFiles/tokenizer-test.output.01 -
	cat ./testFiles/tokenizer-test.input.02 | ./tokenizer | diff ./testFiles/tokenizer-test.output.02 -
	cat ./testFiles/tokenizer-test.input.03 | ./tokenizer | diff ./testFiles/tokenizer-test.output.03 -
	cat ./testFiles/tokenizer-test.input.04 | ./tokenizer | diff ./testFiles/tokenizer-test.output.04 -
	cat ./testFiles/tokenizer-test.input.05 | ./tokenizer | diff ./testFiles/tokenizer-test.output.05 -
	cat ./testFiles/tokenizer-test.input.06 | ./tokenizer | diff ./testFiles/tokenizer-test.output.06 -
	cat ./testFiles/tokenizer-test.input.07 | ./tokenizer | diff ./testFiles/tokenizer-test.output.07 -
	cat ./testFiles/tokenizer-test.input.08 | ./tokenizer | diff ./testFiles/tokenizer-test.output.08 -
	cat ./testFiles/parser-test.input.01 | ./parser | diff ./testFiles/parser-test.output.01 -
	cat ./testFiles/parser-test.input.02 | ./parser | diff ./testFiles/parser-test.output.02 -
	cat ./testFiles/parser-test.input.03 | ./parser | diff ./testFiles/parser-test.output.03 -
	cat ./testFiles/parser-test.input.04 | ./parser | diff ./testFiles/parser-test.output.04 -
	cat ./testFiles/parser-test.input.05 | ./parser | diff ./testFiles/parser-test.output.05 -
	cat ./testFiles/parser-test.input.06 | ./parser | diff ./testFiles/parser-test.output.06 -
	cat ./testFiles/parser-test.input.07 | ./parser | diff ./testFiles/parser-test.output.07 -
	cat ./testFiles/parser-test.input.08 | ./parser | diff ./testFiles/parser-test.output.08 -
	cat ./testFiles/interpreter-test.input.01 | ./interpreter | diff ./testFiles/interpreter-test.output.01 -
	cat ./testFiles/interpreter-test.input.02 | ./interpreter | diff ./testFiles/interpreter-test.output.02 -
	cat ./testFiles/interpreter-test.input.03 | ./interpreter | diff ./testFiles/interpreter-test.output.03 -
	cat ./testFiles/interpreter-test.input.04 | ./interpreter | diff ./testFiles/interpreter-test.output.04 -
	cat ./testFiles/interpreter-test.input.05 | ./interpreter | diff ./testFiles/interpreter-test.output.05 -
	cat ./testFiles/interpreter-test.input.06 | ./interpreter | diff ./testFiles/interpreter-test.output.06 -
	cat ./testFiles/interpreter-test.input.07 | ./interpreter | diff ./testFiles/interpreter-test.output.07 -
	cat ./testFiles/interpreter-test.input.08 | ./interpreter | diff ./testFiles/interpreter-test.output.08 -
	cat ./testFiles/interpreter2-test.input.01 | ./interpreter | diff ./testFiles/interpreter2-test.output.01 -
	cat ./testFiles/interpreter2-test.input.02 | ./interpreter | diff ./testFiles/interpreter2-test.output.02 -
	cat ./testFiles/interpreter2-test.input.03 | ./interpreter | diff ./testFiles/interpreter2-test.output.03 -
	cat ./testFiles/interpreter2-test.input.04 | ./interpreter | diff ./testFiles/interpreter2-test.output.04 -
	cat ./testFiles/interpreter2-test.input.05 | ./interpreter | diff ./testFiles/interpreter2-test.output.05 -
	cat ./testFiles/interpreter2-test.input.06 | ./interpreter | diff ./testFiles/interpreter2-test.output.06 -
	cat ./testFiles/interpreter2-test.input.07 | ./interpreter | diff ./testFiles/interpreter2-test.output.07 -
	cat ./testFiles/interpreter2-test.input.08 | ./interpreter | diff ./testFiles/interpreter2-test.output.08 -
	cat ./testFiles/interpreter3-test.input.01 | ./interpreter | diff ./testFiles/interpreter3-test.output.01 -
	cat ./testFiles/interpreter3-test.input.02 | ./interpreter | diff ./testFiles/interpreter3-test.output.02 -
	cat ./testFiles/interpreter3-test.input.03 | ./interpreter | diff ./testFiles/interpreter3-test.output.03 -
	cat ./testFiles/interpreter3-test.input.04 | ./interpreter | diff ./testFiles/interpreter3-test.output.04 -
	cat ./testFiles/interpreter3-test.input.05 | ./interpreter | diff ./testFiles/interpreter3-test.output.05 -
	cat ./testFiles/interpreter3-test.input.06 | ./interpreter | diff ./testFiles/interpreter3-test.output.06 -
	cat ./testFiles/interpreter3-test.input.07 | ./interpreter | diff ./testFiles/interpreter3-test.output.07 -
	cat ./testFiles/interpreter3-test.input.08 | ./interpreter | diff ./testFiles/interpreter3-test.output.08 -
