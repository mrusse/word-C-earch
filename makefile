CC = gcc
CFLAGS = -std=c99 -Wall -ggdb3
VFLAGS = --show-leak-kinds=all --track-origins=yes --leak-check=full

EXE = wordSearch
OUTPUT = ws.html

$(EXE): wordSearch.c
	$(CC) $(CFLAGS) $^ -o $@

run: $(EXE)
	./$(EXE)

clean:
	rm -rf $(EXE) $(OUTPUT)
