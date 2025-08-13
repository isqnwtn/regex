#pragma once

#include "ast.h"

struct Transition {
public:
    Transition(int from, int to, char label) : from(from), to(to), label(label), is_epsilon(false) {}
    Transition(int from, int to) : from(from), to(to), label('\0'), is_epsilon(true) {}
    int from;
    int to;
    char label;
    bool is_epsilon;
};

class NFA {
public:
    NFA(RegexPtr regex) : regex_(regex), num_states(0) {}
    ~NFA();

    void print();
    bool match(const std::string &input);

private:
    RegexPtr regex_;
    std::vector<std::vector<Transition>> transitions;
    int num_states;
    int build_nfa();
};
