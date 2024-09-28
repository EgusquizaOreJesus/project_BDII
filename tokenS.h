//
// Created by paolo on 17/09/2024.
//

#ifndef PROJECTO1_tokenS_H
#define PROJECTO1_tokenS_H

#include <cctype>
#include <iostream>
#include <string>
#include <map>

using namespace std;

class Token {
public:
    enum Type {
        SELECT, CREATE, TABLE, FROM, ALL, WHERE, DELETE, EQUAL, BETWEEN, AND, INSERT, INTO, VALUES, FILE,
        LPARENT, RPARENT, INDEX, USING, AVL, ISAM, EXTENDIBLE, END, ERR, SEMICOLON, COLON, ID, EOL, NUM, VALUE, QUOTE, FILENAME, TRUE, FALSE, FLOAT
        , SEQUENTIAL
    };
    static const char* token_names[35];
    Type type;
    string lexema;
    Token(Type);
    Token(Type, char c);
    Token(Type, const string& source);
};

std::ostream& operator<<(std::ostream& outs, const Token& tok);
std::ostream& operator<<(std::ostream& outs, const Token* tok);

class ReservedWords {
    map<string, Token::Type> palabras;

public:
    ReservedWords();
    Token::Type search(const string& lexema);
};

class Scanner {
public:
    Scanner(const char* in_s);
    Token* nextToken();
    ~Scanner();

private:
    string input;
    ReservedWords palabras;
    int first, current;
    bool isNumber(const string& s);
    char nextChar();
    void rollBack();
    void startLexema();
    string getLexema();
};

#endif //PROJECTO1_tokenS_H
