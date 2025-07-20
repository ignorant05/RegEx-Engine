run : 
	g++ parser/parser.cpp state/machine.cpp main.cpp -l boost_program_options -o main
	./main
debug: 
	g++ parser/parser.cpp state/machine.cpp main.cpp -l boost_program_options -o main
	gdb ./main
clean: 
	rm main
