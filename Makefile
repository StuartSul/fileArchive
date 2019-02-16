CC = gcc
PROG = Archiver
OBJ = main.o archive.o list.o control.o

$(PROG): $(OBJ)
	$(CC) $(OBJ) -o $(PROG)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJ)

clean_all:
	rm -rf $(PROG) $(OBJ)
