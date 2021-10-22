CC=gcc

LDFLAG=$(shell sdl2-config --cflags --libs) -lm -lSDL2_ttf
CFLAG=-Wall $(shell sdl2-config --cflags --libs) -ggdb3 #pour valgrind détails

EXEC=res
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all:$(EXEC)

$(EXEC):$(OBJ)
	$(CC) -o $@ $^ $(LDFLAG)

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAG)

.PHONY:clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
	rm -rf compile_commands.json
	rm -rfd html/
	rm -rfd latex/
	rm -rf valgrind-out.txt	#clear le fichier de sortie de Valgrind

clangd: clean
	bear make

doxy:
	doxygen Doxyfile

run: $(EXEC)
	./$(EXEC)

