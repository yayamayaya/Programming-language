CC 		= g++
CFLAGS 	= -Wall -Wextra -g -DDEBUG -DGRAPH

l = src/include/lang.h
t = src/include/tokenizator.h
c = ../general/commands/commands.h
g = ../general/graph_creator/create_graph.h
n = ../general/n_tree_func/n_node.h
s = src/include/save_tree.h

fr = ../../common/file_reading/bin/file_reading.o 

lcpp = src/lang_parser/lang_parser.cpp
tcpp = src/lang_parser/tokenizator.cpp
mcpp = src/main.cpp
ccpp = ../general/commands/commands.cpp
gcpp = ../general/graph_creator/create_graph.cpp
ncpp = ../general/n_tree_func/n_node.cpp
scpp = src/save_tree/save_tree.cpp


all: lang_parser.o tokenizator.o main.o commands.o graph.o n_node.o $(fr) save.o
	$(CC) -g -Wall -o bin/front bin/lang_parser.o bin/tokenizator.o bin/main.o bin/commands.o bin/graph.o bin/n_node.o $(fr) bin/save.o

objects = lang_parser.o tokenizator.o main.o

lang_parser.o: $(lcpp) $(l)
	$(CC) $(CFLAGS) -c $< -o bin/$@
tokenizator.o: $(tcpp) $(t)
	$(CC) $(CFLAGS) -c $< -o bin/$@
save.o: $(scpp) $(s)
	$(CC) $(CFLAGS) -c $< -o bin/$@
main.o: $(mcpp)
	$(CC) $(CFLAGS) -c $< -o bin/$@
commands.o:	$(ccpp) $(c)
	$(CC) $(CFLAGS) -c $< -o bin/$@
n_node.o: $(ncpp) $(n)
	$(CC) $(CFLAGS) -c $< -o bin/$@
graph.o: $(gcpp) $(g)
	$(CC) $(CFLAGS) -c $< -o bin/$@

rt:
	valgrind --show-leak-kinds=all --leak-check=full bin/front ../data/tree.txt ../programms/test.txt

rfact:
	valgrind --show-leak-kinds=all --leak-check=full bin/front ../data/tree.txt ../programms/fact.txt

rfibo:
	valgrind --show-leak-kinds=all --leak-check=full bin/front ../data/tree.txt ../programms/fibo.txt

rkv:
	valgrind --show-leak-kinds=all --leak-check=full bin/front ../data/tree.txt ../programms/kvadratka.txt