CC 		= g++
CFLAGS 	= -Wall -Wextra -g -DDEBUG -DGRAPH

c = ../general/commands/commands.h
g = ../general/graph_creator/create_graph.h
n = ../general/n_tree_func/n_node.h
r = src/include/read_tree.h
conv = src/include/bin_2_n.h

fr = ../../common/file_reading/bin/file_reading.o 

mcpp = src/main.cpp
ccpp = ../general/commands/commands.cpp
gcpp = ../general/graph_creator/create_graph.cpp
ncpp = ../general/n_tree_func/n_node.cpp
rcpp = src/read_tree.cpp
convcpp = src/bin_2_n.cpp


all: main.o commands.o graph.o n_node.o $(fr) read.o conv.o
	$(CC) -g -Wall -o bin/front bin/main.o bin/commands.o bin/graph.o bin/n_node.o $(fr) bin/read.o bin/conv.o

objects = lang_parser.o tokenizator.o main.o

main.o: $(mcpp)
	$(CC) $(CFLAGS) -c $< -o bin/$@
commands.o:	$(ccpp) $(c)
	$(CC) $(CFLAGS) -c $< -o bin/$@
n_node.o: $(ncpp) $(n)
	$(CC) $(CFLAGS) -c $< -o bin/$@
graph.o: $(gcpp) $(g)
	$(CC) $(CFLAGS) -c $< -o bin/$@
read.o: $(rcpp) $(g)
	$(CC) $(CFLAGS) -c $< -o bin/$@
conv.o:	$(convcpp) $(conv)
	$(CC) $(CFLAGS) -c $< -o bin/$@

rb:
	valgrind --leak-check=full bin/front ../data/code.txt ../data/tree.txt