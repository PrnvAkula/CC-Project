# Makefile for Bison, Flex, GCC, and Python project

LEX_FILE = part2.l
YACC_FILE = part2.y
C_FILE = part2.c
EXEC = p
INPUT = input.txt
PYTHON_SCRIPT = part2.py

all: $(EXEC)
	@echo "Running compiled executable..."
	./$(EXEC) $(INPUT)
	@echo "Running Python script..."
	python3 $(PYTHON_SCRIPT)

$(EXEC): part2.tab.c lex.yy.c $(C_FILE)
	gcc part2.tab.c lex.yy.c $(C_FILE) -o $(EXEC) -lfl

part2.tab.c part2.tab.h: $(YACC_FILE)
	bison -d $(YACC_FILE)

lex.yy.c: $(LEX_FILE)
	flex $(LEX_FILE)

clean:
	rm -f $(EXEC) part2.tab.* lex.yy.c

.PHONY: all clean
