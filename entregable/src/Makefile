# Compilador que será utilizado
CC=g++

# flags de compilación
CFLAGS=-g -Wall -O2 -std=c++11

# flags de linkeo
LDFLAGS=

# Agrear acá los directorios a incluir en la compilación
INCDIR=. #../../../../../Modulos-CPP

# Agregar acá los archivos .cpp a compilar
SOURCES=test.cpp alto_horno.cpp sistema_ecuaciones.cpp utils.cpp

# Objetos que serán generados (no tocar)
OBJECTS=$(SOURCES:.cpp=.o)

# Nombre del ejecutable a generar
EXECUTABLE=test

.PHONY: all

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): clean $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

tests: $(EXECUTABLE)
	./$(EXECUTABLE)

valgrind: $(EXECUTABLE)
	valgrind --leak-check=full --track-origins=yes ./$(EXECUTABLE)

.cpp.o:
	$(CC) -I$(INCDIR) $(CFLAGS) $< -c -o $@
