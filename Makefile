
OBJECTS = error.o server_com.o server_helper.o game.o ClientUI.o
DEPENDS = error.c server_com.c server_helper.c game.c ClientUI.c
TEST_CLIENT = test_client
CLIENT = client
SERVER = server

CC = gcc
CFLAGS = -Wall -g -std=gnu99 -pedantic
LDLIBS = -lm -lncurses

# default
all: $(TEST_CLIENT) $(CLIENT) $(SERVER)

# test client
$(TEST_CLIENT): $(TEST_CLIENT).o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

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
