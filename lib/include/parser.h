#include <string>
#include "ast.h"

// Helper function to check if a character is an operator
bool isOperator(char c);

// Parser class
class Parser {
public:
    Parser(const std::string& input) : input(input), pos(0) {}

    char peek() const;

    char consume();

    // Helper function to consume a specific character
    bool consumeChar(char expected);

    // Helper function to check if the current character is a digit
    bool isDigit(char c) const;

    const std::string& getInput() const;

    size_t getPos() const;

    RegexPtr parseRegex();
    RegexPtr parseSequence();
    RegexPtr parsePrimary();
    RegexPtr parseRepeatOperator(RegexPtr expr);
    RegexPtr parseAlt(RegexPtr left);
    RegexPtr parseConcat(std::vector<RegexPtr>& seq);
    RegexPtr parseRepeat(RegexPtr expr, RepeatKind kind);
    RegexPtr parseGroup();
    RegexPtr parseWildcard();
    RegexPtr parseChar();

private:
    std::string input;
    size_t pos;
};
