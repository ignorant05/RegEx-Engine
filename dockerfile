FROM gcc:latest

WORKDIR /app

COPY . .

RUN g++ parser/parser.cpp state/machine.cpp main.cpp -l boost_program_options -o regexen 


CMD ["./regexen"]
