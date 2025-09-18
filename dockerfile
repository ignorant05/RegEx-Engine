FROM gcc:latest

RUN apt-get update && apt-get install -y \
	libboost-program-options-dev \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN g++ parser/parser.cpp state/machine.cpp main.cpp -o regexen 

CMD ["./regexen"]
