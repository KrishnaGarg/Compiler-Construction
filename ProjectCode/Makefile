CC = gcc
CFLAGS = -w -g
DEPS = codeGeneration.o TypeCheck.o symbolTable.o AST.o parser.o parseHelper.o lexer.o driver.o

TARGET = compiler1

run: $(TARGET)
	./$(TARGET) test-operators.c

$(TARGET): $(DEPS)
	$(CC) $(CFLAGS) $(DEPS) -o $(TARGET)

codeGeneration.o: codeGeneration.c  
	$(CC) $(CFLAGS) -c codeGeneration.c

TypeCheck.o: TypeCheck.c  
	$(CC) $(CFLAGS) -c TypeCheck.c

symbolTable.o: symbolTable.c  
	$(CC) $(CFLAGS) -c symbolTable.c

AST.o: AST.c  
	$(CC) $(CFLAGS) -c AST.c

parser.o: parser.c  
	$(CC) $(CFLAGS) -c parser.c

parseHelper.o: parseHelper.c  
	$(CC) $(CFLAGS) -c parseHelper.c

dfa.o: dfa.h
	$(CC) $(CFLAGS) -c dfa.c

lexer.o: lexer.c 
	$(CC) $(CFLAGS) -c lexer.c
	
driver.o: driver.c 
	$(CC) $(CFLAGS) -c driver.c
	
clean:
	-rm -f *.o $(TARGET)
