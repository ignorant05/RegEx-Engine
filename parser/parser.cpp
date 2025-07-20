#include "parser.h"

#include <iostream>
#include <set>
#include <stdexcept>

void Parser::appendList(cset *list, char start, char end) {
    for (char c = start; c <= end; ++c) {
        list->insert(c);
    }
}

void Parser::reverseQueue(aqueue *q) {
    astack stk;
    while (!q->empty()) {
        stk.push(q->front());
        q->pop();
    }
    while (!stk.empty()) {
        q->push(stk.top());
        stk.pop();
    }
}

aqueue Parser::parse(Str regex) {
    aqueue q;
    int length = regex.size(), index = 0, lastAlterIndex = index;
    bool grpDone = true;
    bool inAlter = false;

    while (index < length) {
        char c = regex.at(index);

        switch (c) {
            case '^': {
                RegExToken node;
                node.Kind  = StartAnchor;
                node.Quant = One;

                Array state;
                state.Type  = Character;
                state.State = node;
                if (!q.empty()) {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        q.push(last);
                    } else {
                        std::cerr << "Invalid pattern" << std::endl;
                    }
                } else {
                    reverseQueue(&q);
                    q.push(state);
                    grpDone = true;
                }

                break;
            }
            case '$': {
                RegExToken node;
                node.Kind  = EndAnchor;
                node.Quant = One;

                Array state;
                state.Type  = Character;
                state.State = node;

                if (!q.empty()) {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        q.push(last);
                    } else {
                        reverseQueue(&q);
                        q.push(state);
                        grpDone = true;
                    }
                } else {
                    reverseQueue(&q);
                    q.push(state);
                    grpDone = true;
                }
                break;
            }
            case '|': {
                if (q.empty()) break;
                Array alter;
                alter.Type = Alter;
                alter.AlterNum++;
                if (!inAlter) lastAlterIndex = index;
                int i = index;

                Array part;
                Array last = q.front();

                if (!q.empty()) {
                    reverseQueue(&q);
                    if (inAlter) {
                        while (lastAlterIndex < i) {
                            Array last = q.front();
                            q.pop();
                            part.Children->push_back(last);
                            i--;
                        }
                        Array alter = q.front();
                        alter.Children->push_back(part);
                        reverseQueue(&q);
                    } else {
                        Array last = q.front();
                        if (last.Type == Group && !grpDone) {
                            alter.Children = last.Children;
                            last.Children->clear();
                            last.Children->push_back(alter);
                            grpDone = true;
                        } else {
                            while (!q.empty()) {
                                alter.Children->push_back(q.front());
                                q.pop();
                            }
                            q.push(alter);
                            grpDone = false;
                        }
                    }
                    reverseQueue(&q);
                } else {
                    std::cerr << "Invalid character" << std::endl;
                }
                index++;
                break;
            }
            case '?': {
                RegExToken node;
                node.Kind  = Wildcard;
                node.Quant = ZeroOrOne;

                Array state;
                state.Type  = Character;
                state.State = node;
                if (q.empty()) {
                    std::cerr << "Invalid char" << std::endl;
                    break;
                } else {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        state.Children = new std::vector<Array>();
                        state.Children->push_back(last);
                        q.push(last);
                    } else {
                        q.pop();
                        reverseQueue(&q);
                        state.Children = new std::vector<Array>();
                        state.Children->push_back(last);
                        q.push(state);
                        grpDone = true;
                    }
                }
                index++;
                break;
            }
            case '.': {
                RegExToken node;
                node.Kind  = Wildcard;
                node.Quant = One;

                Array state;
                state.Type  = Character;
                state.State = node;

                if (!q.empty()) {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        q.push(last);
                    } else {
                        reverseQueue(&q);
                        q.push(state);
                        grpDone = true;
                    }
                } else {
                    reverseQueue(&q);
                    q.push(state);
                    grpDone = true;
                }
                index++;
                break;
                break;
            }

            case '*': {
                RegExToken node;
                node.Kind  = Wildcard;
                node.Quant = ZeroOrMore;

                Array state;
                state.Type  = Character;
                state.State = node;
                if (q.empty()) {
                    std::cerr << "Invalid char" << std::endl;
                    break;
                } else {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        state.Children = new std::vector<Array>();
                        state.Children->push_back(last);
                        q.push(last);
                    } else {
                        q.pop();
                        reverseQueue(&q);
                        state.Children = new std::vector<Array>();
                        state.Children->push_back(last);
                        q.push(state);
                        grpDone = true;
                    }
                }
                index++;
                break;
            }
            case '+': {
                RegExToken node;
                node.Kind  = Wildcard;
                node.Quant = OneOrMore;

                Array state;
                state.Type  = Character;
                state.State = node;
                if (q.empty()) {
                    std::cerr << "Invalid char" << std::endl;
                    break;
                } else {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        state.Children = new std::vector<Array>();
                        state.Children->push_back(last);
                        q.push(last);
                    } else {
                        q.pop();
                        reverseQueue(&q);
                        state.Children = new std::vector<Array>();
                        state.Children->push_back(last);
                        q.push(state);
                        grpDone = true;
                    }
                }
                index++;
                break;
            }
            case '\r': {
                RegExToken node;
                node.Quant = One;
                node.Kind  = NewLine;

                Array state;
                state.Type  = Character;
                state.State = node;

                if (!q.empty()) {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        q.push(last);
                    } else {
                        reverseQueue(&q);
                        q.push(state);
                        grpDone = true;
                    }
                } else {
                    reverseQueue(&q);
                    q.push(state);
                    grpDone = true;
                }
                index++;
                break;
            }
            case '\n': {
                RegExToken node;
                node.Quant = One;
                node.Kind  = NewLine;

                Array state;
                state.Type  = Character;
                state.State = node;
                if (!q.empty()) {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        q.push(last);
                    } else {
                        reverseQueue(&q);
                        q.push(state);
                        grpDone = true;
                    }
                } else {
                    reverseQueue(&q);
                    q.push(state);
                    grpDone = true;
                }
                index++;
                break;
            }
            case '\t': {
                RegExToken node;
                node.Quant = One;
                node.Kind  = Tab;

                Array state;
                state.Type  = Character;
                state.State = node;

                if (!q.empty()) {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        q.push(last);
                    } else {
                        reverseQueue(&q);
                        q.push(state);
                        grpDone = true;
                    }
                } else {
                    reverseQueue(&q);
                    q.push(state);
                    grpDone = true;
                }
                index++;
                break;
            }
            case '\\': {
                if (index + 1 >= regex.size()) break;
                char next;
                if (q.empty()) break;

                if (index + 1 < length)
                    next = regex.at(index + 1);
                else
                    throw std::runtime_error("Escape character error");

                RegExToken node;
                node.Kind  = Literal;
                node.Quant = One;

                Array state;
                state.Type = Character;
                switch (next) {
                    case 's': {
                        node.Kind   = WhiteSpace;
                        state.Value = ' ';
                        break;
                    }
                    case 'S': {
                        node.Kind = NAWS;
                        break;
                    }
                    case 'w': {
                        node.Kind = Word;
                        break;
                    }
                    case 'W': {
                        node.Kind = NAW;
                        break;
                    }
                    case 'd': {
                        node.Kind = Digit;
                        break;
                    }
                    case 'D': {
                        node.Kind = NAD;
                        break;
                    }
                    default: {
                        throw std::runtime_error("Escape character error");
                        break;
                    }
                }
                state.State = node;

                if (!q.empty()) {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        q.push(last);
                    } else {
                        reverseQueue(&q);
                        q.push(state);
                        grpDone = true;
                    }
                } else {
                    reverseQueue(&q);
                    q.push(state);
                    grpDone = true;
                }
                index++;
                break;
            }
            case '(': {
                Array group;

                grpDone        = false;
                group.Type     = Group;
                group.Children = new avect();

                q.push(group);
                index++;

                break;
            }
            case ')': {
                grpDone = true;
                index++;
                break;
            }
            case '{': {
                int len   = 0;
                int ci    = index + 1;
                char curr = '\0';
                Str dummy = "";
                if (ci < length)
                    curr = regex.at(ci);
                else
                    break;
                Array quan;
                quan.Type = Quantity;
                while (curr != '}') {
                    len++;
                    dummy.push_back(curr);
                    ci++;
                    curr = regex.at(ci);
                }
                if (len == 1 && int(dummy.at(0))) {
                    quan.QuantifierType = Exact;
                    quan.min            = int(dummy.at(0));
                    quan.max            = quan.min;
                } else if (len == 2 && int(dummy.at(0)) && dummy.at(1) == ',') {
                    quan.QuantifierType = Min;
                    quan.min            = int(dummy.at(0));
                } else if (len == 3 && int(dummy.at(0)) && dummy.at(1) == ',' && int(dummy.at(2))) {
                    quan.QuantifierType = Interval;
                    quan.min            = int(dummy.at(0));
                    quan.max            = int(dummy.at(2));
                } else {
                    throw std::runtime_error("invalid");
                }

                if (q.empty()) {
                    std::cerr << "Invalid" << std::endl;
                    break;
                } else {
                    reverseQueue(&q);
                    Array prev = q.front();
                    q.pop();
                    reverseQueue(&q);
                    quan.Children = new std::vector<Array>();
                    quan.Children->push_back(prev);
                }
                index = ci + 1;
                q.push(quan);
                break;
            }
            case '[': {
                Array range;
                range.Type = MatchArray;
                int first = index + 1, mid = index + 2, last = index + 3;

                std::set<char> list = {};
                char start = '\0', middle = '\0', end = '\0';

                if (regex.substr(index, regex.find(']') - index).size() < 3) {
                    list.insert(regex.at(first));
                    list.insert(regex.at(mid));
                } else {
                    if (first < regex.size() && mid < regex.size() && last < regex.size()) {
                        start  = regex.at(first);
                        middle = regex.at(mid);
                        end    = regex.at(last);
                    }

                    while (end != ']') {
                        if (index + 1 == '^') {
                            range.Type = Anti_MatchArray;
                            continue;
                        }
                        if (middle == '-' && start < end) {
                            appendList(&list, start, end);
                        }
                        list.insert(regex.at(last));
                        first++;
                        mid++;
                        last++;

                        if (first < regex.size() && mid < regex.size() && last < regex.size()) {
                            start  = regex.at(first);
                            middle = regex.at(mid);
                            end    = regex.at(last);
                        }
                    }
                    if (index >= length && end != ']') throw std::runtime_error("invalid");
                }

                for (auto i = list.begin(); i != list.end(); i++) {
                    std::cout << *i << std::endl;
                }

                index      = last + 1;
                range.list = list;
                q.push(range);
                break;
            }

            default: {
                Array state;
                state.Type = Character;
                RegExToken node;
                node.Quant  = One;
                node.Kind   = Literal;
                state.State = node;
                state.Value = c;

                if (!q.empty()) {
                    reverseQueue(&q);
                    Array last = q.front();
                    if (last.Type == Group && !grpDone) {
                        if (!last.Children) {
                            avect *vect   = new avect();
                            last.Children = vect;
                        }

                        q.pop();
                        reverseQueue(&q);
                        q.push(last);
                    } else {
                        reverseQueue(&q);
                        q.push(state);
                        grpDone = true;
                    }
                } else {
                    reverseQueue(&q);
                    q.push(state);
                    grpDone = true;
                }
                index++;
                break;
            }
        }
    }
    return q;
}
