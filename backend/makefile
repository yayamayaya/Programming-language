CC 		= g++
CFLAGS 	= -Wall -Wextra -g -DDEBUG

c = ../general/commands/commands.h
g = ../general/graph_creator/create_graph.h
n = ../general/n_tree_func/n_node.h
r = src/include/read_tree.h
conv = src/include/bin_2_n.h
b = src/include/body.h
ef = src/include/trans_expr.h
vf = src/include/trans_variable.h
ar = src/include/arithm.h
f = src/include/trans_func.h

fr = ../../common/file_reading/bin/file_reading.o 

mcpp = src/main.cpp
ccpp = ../general/commands/commands.cpp
gcpp = ../general/graph_creator/create_graph.cpp
ncpp = ../general/n_tree_func/n_node.cpp
rcpp = src/read_tree.cpp
convcpp = src/bin_2_n.cpp
cmcpp = src/translator/translator.cpp
cbcpp = src/translator/trans_body.cpp
efcpp = src/translator/trans_expr.cpp
vfcpp = src/translator/trans_variable.cpp
arcpp = src/translator/arithm_op.cpp
fcpp = src/translator/trans_func.cpp


all: main.o commands.o graph.o n_node.o $(fr) read.o conv.o translator.o trans_body.o expr_func.o var_func.o arithm.o funcs.o ../../stack/bin/functions.o ../../stack/bin/global.o
	$(CC) -g -Wall -o bin/front bin/main.o bin/commands.o bin/graph.o bin/n_node.o $(fr) bin/read.o bin/conv.o bin/translator.o bin/trans_body.o bin/expr_func.o bin/var_func.o bin/arithm.o bin/funcs.o ../../stack/bin/global.o

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
translator.o:	$(cmcpp) $(cm)
	$(CC) $(CFLAGS) -c $< -o bin/$@
trans_body.o: $(cbcpp) $(b)
	$(CC) $(CFLAGS) -c $< -o bin/$@
expr_func.o: $(efcpp) $(ef)
	$(CC) $(CFLAGS) -c $< -o bin/$@
var_func.o:	$(vfcpp) $(vf)
	$(CC) $(CFLAGS) -c $< -o bin/$@
var_func.s: $(vfcpp) $(vf)
	$(CC) $(CFLAGS) -c $< -o bin/$@
arithm.o: $(arcpp) $(ar)
	$(CC) $(CFLAGS) -c $< -o bin/$@
funcs.o: $(fcpp) $(f)
	$(CC) $(CFLAGS) -c $< -o bin/$@

rb:
	valgrind --leak-check=full  bin/front ../data/code.txt ../data/tree.txt