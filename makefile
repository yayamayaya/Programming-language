d = -Wall -g -DDEBUG -DGRAPH

t = src/include/tokenizator.h
n = src/include/n_node.h
c = src/include/tokenizator.h
l = src/include/lang.h
g = graph_creator/create_graph.h
b = src/include/body.h
cm = src/include/compiler.h
cmd = src/include/commands.h
b = src/include/body.h
ef = src/include/expr_func.h
vf = src/include/variable_func.h
ar = src/include/arithm.h

tcpp = src/lang_parser/tokenizator.cpp
ncpp = src/n_tree_func/n_node.cpp
mcpp = src/main.cpp
ccpp = src/commands.cpp
gcpp = graph_creator/create_graph.cpp
pcpp = src/lang_parser/lang_parser.cpp
cmcpp = src/compiler/compiler.cpp
cbcpp = src/compiler/comp_body.cpp
efcpp = src/compiler/expr_func.cpp
vfcpp = src/compiler/variable_func.cpp
arcpp = src/compiler/arithm_op.cpp

all: file_reader.o tokenizator.o node_func.o main.o commands.o parser.o create_graph.o compiler.o compile_body.o expr_func.o var_func.o arithm.o ../stack/bin/functions.o ../stack/bin/global.o
	g++ -Wall -o bin/language bin/file_reader.o bin/tokenizator.o bin/node_func.o bin/main.o bin/commands.o bin/parser.o bin/create_graph.o bin/compiler.o bin/compile_body.o bin/expr_func.o bin/var_func.o bin/arithm.o ../stack/bin/global.o

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

compiler.o:	$(cmcpp) $(cm)
	g++ $(d) -c $(cmcpp) -o bin/compiler.o

create_graph.o: $(gcpp) $(g)
	g++ $(d) -c $(gcpp) -o bin/create_graph.o

parser.o:	$(pcpp) $(l)
	g++ $(d) -c $(pcpp) -o bin/parser.o

compile_body.o: $(cbcpp) $(b)
	g++ $(d) -c $(cbcpp) -o bin/compile_body.o

expr_func.o: $(efcpp) $(ef)
	g++ $(d) -c $(efcpp) -o bin/expr_func.o

var_func.o:	$(vfcpp) $(vf)
	g++ $(d) -c $(vfcpp) -o bin/var_func.o

arithm.o: $(arcpp) $(ar)
	g++ $(d) -c $(arcpp) -o bin/arithm.o

rtt:
	valgrind bin/token_test
rm:
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all bin/language tests/test_programm.txt
rtrt:
	valgrind bin/tree_test