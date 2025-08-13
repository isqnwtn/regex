# include "nfa.h"


void NFA::print() {
    printf("DFA:\n");
    for (auto &state : this->transitions) {
        printf("State %zu:\n", state.size());
        for (auto &transition : state) {
            printf("    %d\n", transition);
        }
    }
}

int NFA::build_nfa() {
    return 0;
}

bool NFA::match(const std::string &input) {
    return false;
}
