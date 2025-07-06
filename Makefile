# Compiler
CC = gcc

# Sources
INCLUDE = ./include
SOURCE = ./src

# Flags
FLAGS = -Wall -std=c99 -pedantic -I$(INCLUDE)

# App name
APP = programaTrab

# All .o files 
OBJECTS = cabecalho.o registro.o erros.o utils.o criterio.o SQL.o indice.o fila.o arvoreb.o

all: $(OBJECTS) main.o
	$(CC) $(FLAGS) $(OBJECTS) main.o -o $(APP)

cabecalho.o: 
	$(CC) $(FLAGS) -c $(SOURCE)/cabecalho.c -o cabecalho.o

registro.o: 
	$(CC) $(FLAGS) -c $(SOURCE)/registro.c -o registro.o

erros.o: 
	$(CC) $(FLAGS) -c $(SOURCE)/erros.c -o erros.o

main.o: 
	$(CC) $(FLAGS) -c $(SOURCE)/main.c -o main.o

utils.o:
	$(CC) $(FLAGS) -c $(SOURCE)/utils.c -o utils.o

criterio.o:
	$(CC) $(FLAGS) -c $(SOURCE)/criterio.c -o criterio.o

SQL.o:
	$(CC) $(FLAGS) -c $(SOURCE)/SQL.c -o SQL.o

indice.o:
	$(CC) $(FLAGS) -c $(SOURCE)/indice.c -o indice.o

arvoreb.o:
	$(CC) $(FLAGS) -c $(SOURCE)/arvoreb.c -o arvoreb.o

fila.o:
	$(CC) $(FLAGS) -c $(SOURCE)/fila.c -o fila.o

run:
	./$(APP)

clean:
	-rm *.o $(APP) indice.bin

indice_test.o:
	gcc $(FLAGS) -c $(SOURCE)/indice_test.c

test: $(OBJECTS) indice_test.o
	$(CC) $(FLAGS) indice_test.o $(OBJECTS) -o test
	
mem_test%: exec/%.in | results
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./programaTrab < $< 2>results/summary$*.log

	