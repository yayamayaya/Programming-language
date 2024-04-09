d = -Wall -g -DDEBUG
t = src/lang_parser/tokenizator.h
n = src/n_tree_func/n_node.h
c = src/include/tokenizator.h
l = src/include/lang.h
cmd = src/include/commands.h
tcpp = src/lang_parser/tokenizator.cpp
ncpp = src/n_tree_func/n_node.cpp
mcpp = src/main.cpp
ccpp = src/commands.cpp

all: file_reader.o tokenizator.o node_func.o main.o commands.o
	g++ -Wall -o bin/language bin/file_reader.o bin/tokenizator.o bin/node_func.o bin/main.o bin/commands.o


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


token_test.o:	src/tests/tokeniz_test.cpp
	g++ -Wall -g $(d) -c src/tests/tokeniz_test.cpp -o bin/token_test.o

token_test:	token_test.o file_reader.o
	g++ -Wall -o bin/token_test bin/file_reader.o bin/token_test.o


rtt:
	valgrind bin/token_test
rm:
	valgrind bin/language tests/test_programm.txt