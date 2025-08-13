#include "ast.h"
#include "parser.h"
#include <iostream>

int main() {
    std::string regex_string = "(a.*(c|d))*|b|e";
    Parser parser(regex_string);
    RegexPtr regex = parser.parseRegex();

    if (regex) {
        std::string dbg = regex->debug_print();
        std::cout << "Parsed regex: \n" << dbg << std::endl;
        // Further processing of the regex node would go here (e.g., printing the AST)
    } else {
        std::cout << "Failed to parse regex: " << regex_string << std::endl;
    }

    return 0;
}
