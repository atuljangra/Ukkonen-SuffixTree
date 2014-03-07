
OBJS = suffixTree.o edge.o node.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
EXEC = suffixTree
all: $(OBJS)
			$(CC) $(LFLAGS) $(OBJS) -o $(EXEC)
		
node.o : node.h node.cpp
			$(CC) $(CFLAGS) node.cpp

edge.o : edge.h edge.cpp 
	    $(CC) $(CFLAGS) edge.cpp

suffixTree.o : suffixTree.h suffixTree.cpp node.h edge.h
	    $(CC) $(CFLAGS) suffixTree.cpp
clean:
	rm -f $(OBJS)
	rm -f $(EXEC)
run:
	./$(EXEC)

