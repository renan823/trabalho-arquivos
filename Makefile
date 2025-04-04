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
OBJECTS = cabecalho.o registro.o buscar.o main.o

all: $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(APP)

cabecalho.o: 
	$(CC) $(FLAGS) $(SOURCE)/cabecalho.c -c cabecalho.o

registro.o: 
	$(CC) $(FLAGS) $(SOURCE)/registro.c -c registro.o

buscar.o: 
	$(CC) $(FLAGS) $(SOURCE)/buscar.c -c buscar.o

main.o: 
	$(CC) $(FLAGS) $(SOURCE)/main.c -c main.o

run:
	./$(APP)

clean:
	rm *.o $(APP)