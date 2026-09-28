CC = gcc
CFLAGS = -Wall -Wextra -g -O0

TARGET = regex_engine
TEST_TARGET = regex_engine_test

SRCS_COMUN = regexc.c regexi.c
OBJS_COMUN = $(SRCS_COMUN:.c=.o)

OBJS = main.o $(OBJS_COMUN)
OBJS_TEST = test.o $(OBJS_COMUN)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

test: (TEST_TARGET)

$(TEST_TARGET): ($OBJS_TEST)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

run_test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean run run_test
