d = -Wall -g -DDEBUG -DGRAPH
#d = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

t = src/include/tokenizator.h
n = src/include/n_node.h
c = src/include/tokenizator.h
l = src/include/lang.h
g = graph_creator/create_graph.h
b = src/include/body.h
cm = src/include/translator.h
cmd = src/include/commands.h
b = src/include/body.h
ef = src/include/trans_expr.h
vf = src/include/trans_variable.h
ar = src/include/arithm.h
f = src/include/trans_func.h

tcpp = src/lang_parser/tokenizator.cpp
ncpp = src/n_tree_func/n_node.cpp
mcpp = src/main.cpp
ccpp = src/commands.cpp
gcpp = graph_creator/create_graph.cpp
pcpp = src/lang_parser/lang_parser.cpp
cmcpp = src/translator/translator.cpp
cbcpp = src/translator/trans_body.cpp
efcpp = src/translator/trans_expr.cpp
vfcpp = src/translator/trans_variable.cpp
arcpp = src/translator/arithm_op.cpp
fcpp = src/translator/trans_func.cpp

all: file_reader.o tokenizator.o node_func.o main.o commands.o parser.o create_graph.o translator.o trans_body.o expr_func.o var_func.o arithm.o ../stack/bin/functions.o ../stack/bin/global.o funcs.o
	g++ -Wall -o bin/language bin/file_reader.o bin/tokenizator.o bin/node_func.o bin/main.o bin/commands.o bin/parser.o bin/create_graph.o bin/translator.o bin/trans_body.o bin/expr_func.o bin/var_func.o bin/funcs.o bin/arithm.o ../stack/bin/global.o

tree_test:	node_func.o tree_t.o create_graph.o
	g++ -Wall -o bin/tree_test bin/node_func.o bin/create_graph.o bin/tree_t.o

token_test:	token_test.o file_reader.o
	g++ -Wall -o bin/token_test bin/file_reader.o bin/token_test.o


file_reader.o:	/home/alexandr/Документы/common/file_reading/src/file_reading.cpp /home/alexandr/Документы/common/file_reading/src/file_reading.h
	g++ $(d) -c /home/alexandr/Документы/common/file_reading/src/file_reading.cpp -o bin/file_reader.o

tokenizator.o: $(tcpp) $(t)
	g++ $(d) -c $(tcpp) -o bin/tokenizator.o

node_func.o: $(ncpp) $(n)
	g++ $(d) -c $(ncpp) -o bin/node_func.o

main.o: $(mcpp) $(l)
	g++ $(d) -c $(mcpp) -o bin/main.o

commands.o: $(ccpp) $(cmd)
	g++ $(d) -c $(ccpp) -o bin/commands.o

translator.o:	$(cmcpp) $(cm)
	g++ $(d) -c $(cmcpp) -o bin/translator.o

create_graph.o: $(gcpp) $(g)
	g++ $(d) -c $(gcpp) -o bin/create_graph.o

parser.o:	$(pcpp) $(l)
	g++ $(d) -c $(pcpp) -o bin/parser.o

trans_body.o: $(cbcpp) $(b)
	g++ $(d) -c $(cbcpp) -o bin/trans_body.o

expr_func.o: $(efcpp) $(ef)
	g++ $(d) -c $(efcpp) -o bin/expr_func.o

var_func.o:	$(vfcpp) $(vf)
	g++ $(d) -c $(vfcpp) -o bin/var_func.o

var_func.s: $(vfcpp) $(vf)
	g++ $(d) -c $(vfcpp) -S var_func.s

arithm.o: $(arcpp) $(ar)
	g++ $(d) -c $(arcpp) -o bin/arithm.o

funcs.o: $(fcpp) $(f)
	g++ $(d) -c $(fcpp) -o bin/funcs.o

rt:
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all bin/language programms/test.txt

rfibo:
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all bin/language programms/fibo.txt

rfact:
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all bin/language programms/fact.txt
