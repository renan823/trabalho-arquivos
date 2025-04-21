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
OBJECTS = cabecalho.o registro.o buscar.o erros.o main.o utils.o

all: $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(APP)

cabecalho.o: 
	$(CC) $(FLAGS) -c $(SOURCE)/cabecalho.c -o cabecalho.o

registro.o: 
	$(CC) $(FLAGS) -c $(SOURCE)/registro.c -o registro.o

buscar.o: 
	$(CC) $(FLAGS) -c $(SOURCE)/buscar.c -o buscar.o

erros.o: 
	$(CC) $(FLAGS) -c $(SOURCE)/erros.c -o erros.o

main.o: 
	$(CC) $(FLAGS) -c $(SOURCE)/main.c -o main.o

utils.o:
	$(CC) $(FLAGS) -c $(SOURCE)/utils.c -o utils.o

run:
	./$(APP)

clean:
	rm *.o $(APP)