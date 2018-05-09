
OBJECTS = error.o server_com.o server_helper.o game.o map.o client_com.o client_helper.o
DEPENDS = error.h server_com.h server_helper.h game.h map.h client_com.h client_helper.h

CLIENT = client
SERVER = server
CC = gcc
CFLAGS = -Wall -g -std=gnu99 -pedantic
LDLIBS = -lm -lncurses -lpthread

# default
all: $(TEST_CLIENT) $(CLIENT) $(SERVER)

# client
$(CLIENT): $(CLIENT).o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# server
$(SERVER): $(SERVER).o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# objects
%.o: %.c $(DEPENDS)
	$(CC) $< -c -o $@ $(CFLAGS)

# clean
clean:
	rm -rf *.o $(TEST_CLIENT) $(CLIENT) $(SERVER)

.PHONY: clean all
