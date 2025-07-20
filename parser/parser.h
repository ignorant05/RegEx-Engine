#ifndef PARSER_H
#define PARSER_H
#include "../utils.h"

class Parser {
   public:
    aqueue parse(Str regex);

   private:
    void reverseQueue(aqueue* q);
    void appendList(cset* list, char start, char end);
};

#endif  // !PARSER_H
