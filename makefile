d = -Wall -g -DDEBUG
t = src/lang_parser/tokenizator.h
n = src/n_tree_func/n_node.h
c = src/include/tokenizator.h
l = src/include/lang.h
g = graph_creator/create_graph.h
cmd = src/include/commands.h
tcpp = src/lang_parser/tokenizator.cpp
ncpp = src/n_tree_func/n_node.cpp
mcpp = src/main.cpp
ccpp = src/commands.cpp
gcpp = graph_creator/create_graph.cpp
pcpp = src/lang_parser/lang_parser.cpp

all: file_reader.o tokenizator.o node_func.o main.o commands.o parser.o create_graph.o
	g++ -Wall -o bin/language bin/file_reader.o bin/tokenizator.o bin/node_func.o bin/main.o bin/commands.o bin/parser.o bin/create_graph.o

tree_test:	node_func.o tree_t.o create_graph.o
	g++ -Wall -o bin/tree_test bin/node_func.o bin/create_graph.o bin/tree_t.o

token_test:	token_test.o file_reader.o
	g++ -Wall -o bin/token_test bin/file_reader.o bin/token_test.o


file_reader.o:	/home/alexandr/Документы/common/file_reading/file_reader.cpp /home/alexandr/Документы/common/file_reading/file_reader.h
	g++ $(d) -c /home/alexandr/Документы/common/file_reading/file_reader.cpp -o bin/file_reader.o

tokenizator.o: $(tcpp) $(t)
	g++ $(d) -c $(tcpp) -o bin/tokenizator.o

node_func.o: $(ncpp) $(n)
	g++ $(d) -c $(ncpp) -o bin/node_func.o

main.o: $(mcpp) $(l)
	g++ $(d) -c $(mcpp) -o bin/main.o

commands.o: $(ccpp) $(cmd)
	g++ $(d) -c $(ccpp) -o bin/commands.o

create_graph.o: $(gcpp) $(g)
	g++ $(d) -c $(gcpp) -o bin/create_graph.o

tree_t.o: tests/tree_test.cpp
	g++ $(d) -c tests/tree_test.cpp -o bin/tree_t.o

token_test.o:	tests/tokeniz_test.cpp
	g++ $(d) -c tests/tokeniz_test.cpp -o bin/token_test.o

parser.o:	$(pcpp) $(l)
	g++ $(d) -c $(pcpp) -o bin/parser.o


rtt:
	valgrind bin/token_test
rm:
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all bin/language tests/test_programm.txt
rtrt:
	valgrind bin/tree_test