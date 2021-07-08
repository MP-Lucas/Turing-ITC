MAIN=TuringMachine.c
BINARY=main

all:
	gcc -Wall $(MAIN) -o $(BINARY)

run:
	./$(BINARY)

debug:
	gcc -DDEBUG -Wall $(MAIN) -o $(BINARY)

clean:
	@rm *.o

