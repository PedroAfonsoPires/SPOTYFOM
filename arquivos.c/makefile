CC = gcc
CFLAGS = -Wall -Wextra -g3
TARGET = output/main.exe
SOURCES = musica.c main.c
HEADERS = ../arquivos.h/bibliotecas.h ../arquivos.h/musica.h

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)