#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

enum Content {
    Empty,
    Group,
    MatchArray,
    Anti_MatchArray,
    Quantity,
    Character,
    Alter,
};

enum QuantifType {
    Exact,
    Min,
    Interval,
    NotValid,
};

enum TokenType {
    None,
    Tab,
    NewLine,
    Word,
    NAW,
    WhiteSpace,
    NAWS,
    Digit,
    NAD,
    EP,
    Wildcard,
    Literal,
    StartAnchor,
    EndAnchor,
    Comma,
    Dash
};

enum QuantifierType { QuantifierNone, ZeroOrOne, ZeroOrMore, One, OneOrMore, Range };

struct RegExToken {
    TokenType Kind       = None;
    QuantifierType Quant = QuantifierNone;

    bool isEmpty() const { return Kind == None; }
};

struct Array {
    RegExToken State             = RegExToken{};
    char Value                   = '\0';
    std::vector<char> Range      = {};
    Content Type                 = Empty;
    std::set<char> list          = {};
    int min                      = INT_MIN;
    int max                      = INT_MAX;
    int GroupLevel               = 1;
    int AlterNum                 = 0;
    QuantifType QuantifierType   = NotValid;
    std::vector<Array>* Children = nullptr;
};

using svect  = std::vector<std::string>;
using cvect  = std::vector<char>;
using cset   = std::set<char>;
using avect  = std::vector<Array>;
using astack = std::stack<Array>;
using aqueue = std::queue<Array>;
using Str    = std::string;
#endif  // !UTILS_H
