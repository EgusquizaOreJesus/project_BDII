//
// Created by paolo on 17/09/2024.
//

#include "tokenS.h"
#include <cstring>

const char* Token::token_names[35] = {
        "SELECT", "CREATE", "TABLE", "FROM", "ALL", "WHERE", "DELETE", "EQUAL", "BETWEEN", "AND",
        "INSERT", "INTO", "VALUES", "FILE", "LPARENT", "RPARENT", "INDEX", "USING", "AVL",
         "EXTENDIBLE", "END", "ERR", "SEMICOLON", "COLON", "ID", "EOL", "NUM", "VALUE",
        "QUOTE", "FILENAME", "TRUE", "FALSE", "FLOAT", "SEQUENTIAL"
};

Token::Token(Type type) : type(type) { lexema = ""; }
Token::Token(Type type, const std::string& source) : type(type) { lexema = source; }
Token::Token(Type type, char c) : type(type) { lexema = std::string(1, c); }

std::ostream& operator<<(std::ostream& outs, const Token& tok) {
    if (tok.lexema.empty())
        return outs << Token::token_names[tok.type];
    else
        return outs << Token::token_names[tok.type] << "(" << tok.lexema << ")";
}

std::ostream& operator<<(std::ostream& outs, const Token* tok) {
    return outs << *tok;
}

ReservedWords::ReservedWords() {
    palabras.insert(std::make_pair("SELECT", Token::SELECT));
    palabras.insert(std::make_pair("CREATE", Token::CREATE));
    palabras.insert(std::make_pair("TABLE", Token::TABLE));
    palabras.insert(std::make_pair("FROM", Token::FROM));
    palabras.insert(std::make_pair("WHERE", Token::WHERE));
    palabras.insert(std::make_pair("DELETE", Token::DELETE));
    palabras.insert(std::make_pair("BETWEEN", Token::BETWEEN));
    palabras.insert(std::make_pair("AND", Token::AND));
    palabras.insert(std::make_pair("INSERT", Token::INSERT));
    palabras.insert(std::make_pair("INTO", Token::INTO));
    palabras.insert(std::make_pair("VALUES", Token::VALUES));
    palabras.insert(std::make_pair("FILE", Token::FILE));
    palabras.insert(std::make_pair("INDEX", Token::INDEX));
    palabras.insert(std::make_pair("USING", Token::USING));
    palabras.insert(std::make_pair("AVL", Token::AVL));
    palabras.insert(std::make_pair("EXTENDIBLE", Token::EXTENDIBLE));
    palabras.insert(std::make_pair("SEQUENTIAL", Token::SEQUENTIAL));
}

Token::Type ReservedWords::search(const std::string& lexema) {
    auto it = palabras.find(lexema);
    if (it != palabras.end()) return it->second;
    return Token::ID;
}

Scanner::Scanner(const char* s) : input(s), first(0), current(0) { }

Token* Scanner::nextToken() {
    Token* token;
    char c;

    // Consume whitespaces
    do {
        c = nextChar();
    } while (c == ' ' || c == '\t' || c == '\n');
    if (c == '\0') return new Token(Token::END);

    startLexema();
    if (isdigit(c) || isalpha(c) || c == '_') {
        c = nextChar();
        while (isalpha(c) || isdigit(c) || c == '_') {
            c = nextChar();
        }
        rollBack();
        std::string lexema = getLexema();
        Token::Type type = palabras.search(lexema);
        if (type != Token::ID) {
            return new Token(type, lexema);
        } else if (isNumber(lexema)) {
            // AVANZA HASTA QUE NO SEA UN DIGITO
            return new Token(Token::NUM, lexema);
        } else {
            return new Token(Token::ID, lexema);
        }
    } else if (c == '\'') {

        startLexema();
        c = nextChar();
        while (c != '\'' && c != '\0') {
            c = nextChar();
        }
        if (c == '\'') {
            std::string lexema = getLexema();
            return new Token(Token::VALUE, lexema.substr(1, lexema.length() - 2));
        } else {
            return new Token(Token::ERR, c);
        }
    } else if (strchr("()=,;*'", c)) {
        switch (c) {
            case '(': return new Token(Token::LPARENT);
            case ')': return new Token(Token::RPARENT);
            case '=': return new Token(Token::EQUAL);
            case ',': return new Token(Token::COLON);
            case ';': return new Token(Token::SEMICOLON);
            case '*': return new Token(Token::ALL);
            case '\'': return new Token(Token::QUOTE);
            default: return new Token(Token::ERR, c);
        }
    } else {
        return new Token(Token::ERR, c);
    }
}

Scanner::~Scanner() { }

bool Scanner::isNumber(const std::string& s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

char Scanner::nextChar() {
    if (current < input.length()) {
        return input[current++];
    } else {
        return '\0';
    }
}

void Scanner::rollBack() {
    if (current > 0) current--;
}

void Scanner::startLexema() {
    first = current - 1;
}

std::string Scanner::getLexema() {
    return input.substr(first, current - first);
}
