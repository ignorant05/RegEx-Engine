
#include "machine.h"

#include <climits>
#include <iostream>
#include <set>
#include <vector>

Array Machine::popAndReturn(avect* array) {
    Array node = array->back();
    array->pop_back();
    return node;
}

Array Machine::popFrontAndReturn(avect* array) {
    Array node = array->front();
    array->erase(array->begin());
    return node;
}

int Machine::accumulation(int index, char comp, Str target, int current) {
    if (index >= target.size()) return current;

    if (target.at(index) == comp) {
        return accumulation(index + 1, comp, target, current++);
    }

    return current;
}

bool Machine::compare(int num, int min, int max, QuantifType type) {
    switch (type) {
        case Exact: {
            if (num == min)
                return true;
            else
                return false;
            break;
        }
        case Min: {
            if (num >= min)
                return true;
            else
                return false;
            break;
        }
        case Interval: {
            if (max > min || num > max || min > num)
                return false;
            else
                return true;
            break;
        }
        default: {
            std::cerr << "Invalid input" << std::endl;
            return false;
            break;
        }
    }

    return false;
}

aqueue Machine::copy(aqueue original) {
    aqueue nq;

    while (!original.empty()) {
        nq.push(original.front());
        original.pop();
    }

    return nq;
}
int Machine::countSpaces(Str target, int index) {
    int number = 0;
    while (index < target.size()) {
        if (target.at(index) == ' ') {
            number++;
        } else
            break;

        index++;
    }
    return number;
}

void Machine::backtrack(bool isGroup, QuantifType quantiferType, bool isOneOrMore, bool isZeroOrOne,
                        svect& result, int currentProgress, Str& str, const aqueue* original,
                        aqueue* q, Array child, Str target, char previousChar, int index, int min,
                        int max, int num) {
    auto nq  = copy(*original);
    auto org = copy(*original);

    if (!isGroup && q->empty() && !isZeroOrOne) {
        if (!str.empty()) {
            result.push_back(str);
            backtrack(isGroup, NotValid, false, false, result, 0, str = "", &org, &nq,
                      child = Array(), target, '\0', index + 1, min, max, 0);
        }
        return;
    }
    if (index >= target.size() || (index + 1 == target.size() && isZeroOrOne)) {
        std::cout << "done" << std::endl;
        return;
    }

    if (quantiferType != NotValid) {
        if (!compare(num, min, max, quantiferType)) {
            num++;
        } else {
            backtrack(isGroup, NotValid, false, false, result, 0, str = "", &org, &nq,
                      child = Array(), target, '\0', index + 1, min, max, 0);
        }
    }

    Array regex;
    if (!isGroup && q->empty() && !isZeroOrOne) {
        return;
    } else if (!q->empty()) {
        regex = q->front();
        q->pop();
    }
    if (isZeroOrOne) std::cout << "second current string: " << str << std::endl;

    auto type = regex.Type;

    if (previousChar == '$') {
        if (str.at(str.size() - 1) != target.at(target.size() - 1)) {
            std::cerr << "Invalid" << std::endl;
            result = {};
        }
        return;
    }

    if (index > 0) {
        previousChar = target.at(index - 1);
    }

    if (isZeroOrOne) {
        regex = child;
        std::cout << "regex: " << regex.Value << std::endl;
    }

    switch (type) {
        case Character: {
            auto kind = regex.State.Kind;

            switch (kind) {
                case StartAnchor: {
                    if (!q->empty())
                        backtrack(isGroup, NotValid, false, false, result, 0, str, &org, q, regex,
                                  target, '^', index, min, max, num);

                    break;
                }
                case EndAnchor: {
                    if (!str.empty() && str.at(str.size() - 1))
                        backtrack(isGroup, NotValid, false, false, result, 0, str, &org, q,
                                  org.back(), target, '$', 0, min, max, num);

                    break;
                }
                case Tab: {
                    int tab = countSpaces(target, index);
                    if (target.at(index) == '\t') {
                        str.push_back('\t');
                        if (!q->empty())
                            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1,
                                      str, &org, q, regex, target, '\t', index + tab, min, max,
                                      num);
                    } else if (tab >= 4) {
                        while (tab > 0) {
                            str.push_back(' ');
                            if (!q->empty())
                                backtrack(isGroup, NotValid, false, false, result,
                                          currentProgress + 1, str, &org, q, regex, target, '\t',
                                          index + tab, min, max, num);
                        }

                    } else {
                        backtrack(isGroup, NotValid, false, false, result, 0, str = "", &org, &nq,
                                  child = Array(), target, previousChar = '\0', index + 1, min, max,
                                  0);
                    }

                    break;
                }
                case NewLine: {
                    if (target.at(index) == '\n') {
                        str.push_back('\n');
                        if (!q->empty())
                            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1,
                                      str, &org, q, regex, target, '\n', index + 1, min, max, num);

                    } else {
                        backtrack(isGroup, NotValid, false, false, result, 0, str = "", &org, &nq,
                                  child = Array(), target, previousChar = '\0', index + 1, min, max,
                                  0);
                    }

                    break;
                }
                case NAD: {
                    if (!int(target.at(index))) {
                        str.push_back(target.at(index));
                        if (!q->empty())
                            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1,
                                      str, &org, q, regex, target, previousChar, index + 1, min,
                                      max, num);

                    } else {
                        backtrack(isGroup, NotValid, false, false, result, 0, str = "", &org, &nq,
                                  child = Array(), target, previousChar = '\0', index + 1, min, max,
                                  0);
                    }

                    break;
                }
                case WhiteSpace: {
                    if (target.at(index) == regex.Value) {
                        str.push_back(' ');
                        if (!q->empty()) q->pop();
                        backtrack(isGroup, NotValid, false, false, result, currentProgress + 1, str,
                                  &org, q, regex, target, ' ', index + 1, min, max, num);

                    } else
                        backtrack(isGroup, NotValid, false, false, result, 0, str = "", &org, &nq,
                                  Array(), target, '\0', index + 1, min, max, 0);
                    break;
                }
                case NAWS: {
                    if (target.at(index) != ' ') {
                        str.push_back(target.at(index));
                        if (!q->empty()) q->pop();
                        backtrack(isGroup, NotValid, false, false, result, currentProgress + 1, str,
                                  &org, q, regex, target, previousChar, index + 1, min, max, num);

                    } else {
                        backtrack(isGroup, NotValid, false, false, result, 0, str = "", &org, &nq,
                                  child = Array(), target, previousChar = '\0', index + 1, min, max,
                                  0);
                    }

                    break;
                }
                case Literal: {
                    if (regex.Value == '\0') return;
                    std::cout << "index: " << index << "char: " << target.at(index) << std::endl;

                    if (index < target.size() && target.at(index) == child.Value) {
                        str.push_back(target.at(index));
                        backtrack(isGroup, NotValid, false, false, result, currentProgress + 1, str,
                                  &org, q, child, target, previousChar, index + 1, min, max, num);

                    } else if (!isZeroOrOne && target.at(index) == regex.Value) {
                        str.push_back(target.at(index));
                        if (!q->empty())
                            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1,
                                      str, &org, q, q->front(), target, '\0', index + 1, min, max,
                                      num);

                    } else if (target.at(index) == regex.Value) {
                        if (previousChar == '^' && index != 0) {
                            return;
                        }

                        if (!nq.empty())
                            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1,
                                      str, &org, q, nq.front(), target, '\0', index + 1, min, max,
                                      num);

                    } else {
                        backtrack(isGroup, NotValid, false, false, result, 0, str = "", &org, &nq,
                                  child = Array(), target, previousChar = '\0', index + 1, min, max,
                                  0);
                    }
                    break;
                }
                case Wildcard: {
                    auto quant = regex.State.Quant;

                    switch (quant) {
                        case ZeroOrMore: {
                            if (index == 0) return;
                            auto child = regex.Children->front();

                            while (index < target.size() && target.at(index) == child.Value) {
                                str.push_back(target.at(index));
                                backtrack(isGroup, NotValid, false, true, result,
                                          currentProgress + 1, str, &org, q, child, target,
                                          previousChar, index + 1, min, max, num);
                                index++;
                            }

                            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1,
                                      str, &org, q, child, target, previousChar, index, min, max,
                                      0);
                            break;
                        }
                        case ZeroOrOne: {
                            if (index == 0) return;
                            auto child = regex.Children->front();

                            if (index < target.size() && target.at(index) == child.Value) {
                                str.push_back(target.at(index));
                                backtrack(isGroup, NotValid, false, false, result,
                                          currentProgress + 1, str, &org, q, child, target,
                                          previousChar, index + 1, min, max, num);
                            }

                            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1,
                                      str, &org, q, child, target, previousChar, index, min, max,
                                      0);
                            break;
                        }
                        case One: {
                            if (target.at(index) != '\n') {
                                str.push_back(target.at(index));
                                if (!q->empty()) q->pop();
                                backtrack(isGroup, NotValid, false, false, result,
                                          currentProgress + 1, str, &org, q, regex, target,
                                          previousChar = '\0', index + 1, min, max, num);

                            } else {
                                backtrack(isGroup, NotValid, false, false, result, 0, str = "",
                                          &org, &nq, child = Array(), target, previousChar = '\0',
                                          index + 1, min, max, 0);
                            }
                            break;
                        }
                        case OneOrMore: {
                            if (index == 0) return;
                            auto child = regex.Children->front();
                            if (index < target.size() && target.at(index) == child.Value) {
                                str.push_back(target.at(index));
                                backtrack(isGroup, NotValid, false, true, result,
                                          currentProgress + 1, str, &org, q, child, target,
                                          previousChar, index + 1, min, max, num);
                                index++;
                            } else
                                return;

                            while (index < target.size() && target.at(index) == child.Value) {
                                str.push_back(target.at(index));
                                backtrack(isGroup, NotValid, false, true, result,
                                          currentProgress + 1, str, &org, q, child, target,
                                          previousChar, index + 1, min, max, num);
                                index++;
                            }

                            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1,
                                      str, &org, q, child, target, previousChar, index, min, max,
                                      0);
                            break;
                        }
                        default: {
                            if (!nq.empty())
                                backtrack(isGroup, NotValid, false, false, result, 0, str, &org,
                                          &nq, nq.front(), target, '\0', index + 1, min, max, num);
                            break;
                        }
                    }
                    break;
                }
                default: {
                    return;
                }
            }
        }
        case Group: {
            while (regex.Children != nullptr && !regex.Children->empty()) {
                auto child = popFrontAndReturn(regex.Children);
                backtrack(true, NotValid, false, true, result, currentProgress + 1, str, &org, {},
                          child, target, previousChar, index, min, max, num);
                index++;
            }

            break;
        }
        case MatchArray: {
            auto assemble = regex.list;
            if (index < target.size() && assemble.find(target.at(index)) != assemble.end()) {
                str.push_back(target.at(index));
            }
            if (!q->empty()) q->pop();
            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1, str, &org, q,
                      regex, target, previousChar, index + 1, min, max, 0);

            break;
        }
        case Anti_MatchArray: {
            auto assemble = regex.list;
            if (index < target.size() && assemble.find(target.at(index)) == assemble.end()) {
                str.push_back(target.at(index));
            }
            if (!q->empty()) q->pop();
            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1, str, &org, q,
                      regex, target, previousChar, index + 1, min, max, 0);

            break;
        }
        case Quantity: {
            int min = regex.min, max = regex.max, len = target.size();
            auto child = regex.Children->front();
            int n      = num > 0 ? num : 0;

            if (index < target.size()) {
                str.push_back(target.at(index));
                backtrack(isGroup, regex.QuantifierType, false, false, result, currentProgress, str,
                          &org, q, child, target, previousChar, index + 1, min, max, n + 1);
            }

            break;
        }
        case Alter: {
            while (regex.Children != nullptr && !regex.Children->empty()) {
                auto child = popFrontAndReturn(regex.Children);
                backtrack(isGroup, NotValid, false, true, result, currentProgress + 1, str, &org, q,
                          child, target, previousChar, index + 1, min, max, num);
            }
            backtrack(isGroup, NotValid, false, false, result, currentProgress + 1, str, &org, q,
                      child, target, previousChar, index, min, max, 0);
            break;
        }
        default: {
            return;
        }
    }
}

svect Machine::marche(aqueue* q, const Str& target) {
    svect result = {};
    Str str;
    if (q->empty()) return {};
    const auto org = q;

    backtrack(false, NotValid, false, false, result, 0, str, org, q, Array(), target, '\0', 0,
              INT_MIN, INT_MAX, 0);
    return result;
}
