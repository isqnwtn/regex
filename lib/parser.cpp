#include <string>
#include <memory>
#include <vector>
#include "include/ast.h" // Assuming the ast.h file is in the correct path
#include "include/parser.h"

RegexPtr parseRegex(Parser& parser);

// Helper function to check if a character is an operator
bool isOperator(char c) {
    return c == '*' || c == '+' || c == '?' || c == '(' || c == ')' || c == '|';
}

// Parser class defs
char Parser::peek() const {
    if (pos < input.length()) {
        return input[pos];
    }
    return '\0';
}

char Parser::consume() {
    if (pos < input.length()) {
        return input[pos++];
    }
    return '\0';
}

// Helper function to consume a specific character
bool Parser::consumeChar(char expected) {
    if (peek() == expected) {
        consume();
        return true;
    }
    return false;
}

// Helper function to check if the current character is a digit
bool Parser::isDigit(char c) const {
    return c >= '0' && c <= '9';
}


const std::string& Parser::getInput() const {
    return input;
}


size_t Parser::getPos() const {
    return pos;
}



RegexPtr Parser::parseChar() {
    char c = this->peek();
    if (c != '\0' && !isOperator(c)) {
        char res = this->consume();
        return std::make_shared<RegexNode>(CharNode{res});
    }
    return nullptr;
}

RegexPtr Parser::parseWildcard() {
    char c = this->peek();
    if (c ==  '.') {
        this->consume();
        return std::make_shared<RegexNode>(WildCardnode{});
    }
    return nullptr;
}

RegexPtr Parser::parseGroup() {
    if (this->consumeChar('(')) {
        RegexPtr node = this->parseRegex();
        if (node && this->consumeChar(')')) {
            return std::make_shared<RegexNode>(GroupNode{node});
        }
    }
    return nullptr;
}

RegexPtr Parser::parseRepeat(RegexPtr expr, RepeatKind kind) {
    return std::make_shared<RegexNode>(RepeatNode{expr, kind});
}

RegexPtr Parser::parseConcat(std::vector<RegexPtr>& seq) {
    if (seq.empty()) {
        return nullptr;
    }
    if (seq.size() == 1) {
        return seq[0];
    }
    return std::make_shared<RegexNode>(ConcatNode{seq});
}

RegexPtr Parser::parseAlt(RegexPtr left) {
    if (this->consumeChar('|')) {
        RegexPtr right = this->parseRegex();
        if (right) {
            return std::make_shared<RegexNode>(AltNode{left, right});
        }
    }
    return left;
}


RegexPtr Parser::parseRepeatOperator(RegexPtr expr) {
    if (this->consumeChar('*')) {
        return this->parseRepeat(expr, RepeatKind::ZeroOrMore);
    }
    if (this->consumeChar('+')) {
        return this->parseRepeat(expr, RepeatKind::OneOrMore);
    }
    if (this->consumeChar('?')) {
        return this->parseRepeat(expr, RepeatKind::Optional);
    }
    return expr;
}


RegexPtr Parser::parsePrimary() {
    RegexPtr node = this->parseWildcard();
    if (node) return node;

    node = this->parseChar();
    if (node) return node;

    node = this->parseGroup();
    if (node) return node;

    return nullptr;
}

RegexPtr Parser::parseSequence() {
    std::vector<RegexPtr> sequence;
    RegexPtr term;

    while ((term = this->parsePrimary()) != nullptr) {
        term = this->parseRepeatOperator(term);
        sequence.push_back(term);
    }
    return this->parseConcat(sequence);
}

RegexPtr Parser::parseRegex() {
    RegexPtr left = this->parseSequence();
    if (left) {
        return this->parseAlt(left);
    }
    return nullptr;
}
