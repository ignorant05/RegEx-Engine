#ifndef MACHINE_H
#define MACHINE_H

#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "../utils.h"

using svect  = std::vector<std::string>;
using avect  = std::vector<Array>;
using astack = std::stack<Array>;
using aqueue = std::queue<Array>;
using Str    = std::string;

class Machine {
   public:
    svect marche(aqueue* regex, const Str& target);

   private:
    void backtrack(bool isGroup, QuantifType quantifierType, bool isOneOrMore, bool isZeroOrOne,
                   svect& result, int currentProgress, Str& str, const aqueue* original, aqueue* q,
                   Array child, Str target, char previousChar, int index, int min, int max,
                   int num = 0);
    bool compare(int num, int min, int max, QuantifType type);

    int accumulation(int index, char comp, Str target, int current);
    int countSpaces(Str target, int index);

    Array popAndReturn(avect* array);
    Array popFrontAndReturn(avect* array);
    aqueue copy(aqueue original);
};

#endif  // !MACHINE_H
