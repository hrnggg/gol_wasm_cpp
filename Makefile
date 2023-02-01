CC=g++
CFLAGS=-std=c++17 -O3
SRCS=main.cpp
OBJS=$(SRCS:%.cpp=%.o)

# automatically generate target files if it does not exist or is older than files it depens on.
build: $(OBJS)
	$(CC) $(CFLAGS) -o main.out $(OBJS)

# $<: dependency
# $@: target
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	./main.out

clean:
	rm -f *.o *.out

.PHONY: build run clean
