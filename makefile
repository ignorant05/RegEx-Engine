run : 
	g++ parser/parser.cpp state/machine.cpp main.cpp -l boost_program_options -o regexen 
	./regexen
debug: 
	g++ parser/parser.cpp state/machine.cpp main.cpp -l boost_program_options -o regexen
	gdb ./regexen
clean: 
	rm regexen
