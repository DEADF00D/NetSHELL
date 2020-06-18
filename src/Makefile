CC=gcc
CFLAGS=-W -Wall
LDFLAGS=-lpthread
EXEC=ns

all: $(EXEC)

ns: connection.o terminal.o utils.o handlers.o main.o arguments.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper:
	rm -rf $(EXEC)
