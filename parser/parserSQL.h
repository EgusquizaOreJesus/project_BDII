//
// Created by paolo on 17/09/2024.
//

#ifndef PROJECTO1_PARSERSQL_H
#define PROJECTO1_PARSERSQL_H

#include <iostream>
#include <vector>
#include <string>
#include "tokenS.h"
#include "avlFile.h"

using namespace std;

struct Table {
    string name;
    string file;
    string index;
};

struct Condition {
    string field;
    string op;
    string value1;
    string value2;
};

class Parser {
public:

    Parser(Scanner* scanner, AVLFile<const char*>* avlFileApp, AVLFile<int>* avlFileAnime);

    void parse();

private:
    Scanner* scanner;
    Token* currentToken;
    vector<Table> tables;
    AVLFile<const char*>* avlFileApp;  // apps
    AVLFile<int>* avlFileAnime;        // anime

    void parseStatement();
    void parseCreateTable();
    void parseSelect();
    void parseInsert();
    void parseDelete();
    Condition parseCondition();
    vector<string> parseValues();
    Token* expect(Token::Type type);
    Token* expectOneOf(const std::initializer_list<Token::Type>& types);
    void error(const string& message);
};

#endif //PROJECTO1_PARSERSQL_H
