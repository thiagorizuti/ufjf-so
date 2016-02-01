CC = gcc
CFLAGS = -Wall -g
SOURCES = main.c lottery.c process.c sched.c
HEADERS = lottery.h process.h sched.h
OBJECTS = main.o lottery.o process.o sched.o
EXECUTABLE = lottery_scheduling

all: $(EXECUTABLE) run clean

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main.o: $(HEADERS)

lottery.o: lottery.c lottery.h

process.o: $(HEADERS)

sched.o: sched.c sched.h

clean: 
	rm -f $(EXECUTABLE) 
	rm -f $(OBJECTS)

run:
	./$(EXECUTABLE)

.PHONY: all clean run
