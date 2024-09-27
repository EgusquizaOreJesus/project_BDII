// Created by paolo on 17/09/2024.
//

#ifndef PROJECTO1_PARSERSQL_H
#define PROJECTO1_PARSERSQL_H

#include <iostream>
#include <vector>
#include <string>
#include "tokenS.h"
#include "avlFile.h"
#include "ExtendibleHashing.h"

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
template<class TK>
class Parser {
public:

    Parser(Scanner* scanner, const char* estructura);
    Parser(Scanner* scanner);
    void parse();
    char fileStructure[20];


    const char* getStructure(){
        return fileStructure;
    }
private:
    Scanner* scanner;
    Token* currentToken;
    vector<Table> tables;
    AVLFile<const char*>* avlYoutube;  // apps
    AVLFile<int>* avlApps;               // youtube
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

template<class TK>
Parser<TK>::Parser(Scanner *scanner) {
    this->scanner = scanner;
    avlApps = nullptr;
    avlYoutube = nullptr;
    currentToken = scanner->nextToken();
}

template<class TK>
Parser<TK>::Parser(Scanner *scanner, const char *estructura) {
    this->scanner = scanner;
    if (strcmp(estructura, "avlFileApps") == 0){
        avlApps = new AVLFile<int>("playstore.dat");
        avlYoutube = nullptr;
    }
    else if (strcmp(estructura, "avlFileYoutube") == 0){
        avlYoutube = new AVLFile<const char*>("youtube_data.bin");
        avlApps = nullptr;
    }
    currentToken = scanner->nextToken();
}

template<class TK>
Token *Parser<TK>::expectOneOf(const initializer_list<Token::Type> &types) {
    for (Token::Type type : types) {
        if (currentToken->type == type) {
            Token* token = currentToken;
            currentToken = scanner->nextToken();
            return token;
        }
    }
    error("Expected one of the following tokens: ");
    for (Token::Type type : types) {
        error(string(Token::token_names[type]) + " ");
    }
    return nullptr;
}
template<class TK>
void Parser<TK>::error(const string &message) {

}

template<class TK>
void Parser<TK>::parse() {
    while (currentToken->type != Token::END) {
        parseStatement();
        if (currentToken->type == Token::SEMICOLON) {
            currentToken = scanner->nextToken();
        } else if (currentToken->type != Token::END) {
            error("Expected token: SEMICOLON but got: " + string(Token::token_names[currentToken->type]));
        }
    }
}

template<class TK>
void Parser<TK>::parseStatement() {
    if (currentToken->type == Token::CREATE) {
        parseCreateTable();
    } else if (currentToken->type == Token::SELECT) {
        parseSelect();
    } else if (currentToken->type == Token::INSERT) {
        parseInsert();
    } else if (currentToken->type == Token::DELETE) {
        parseDelete();
    } else {
        error("Unexpected token: " + string(Token::token_names[currentToken->type]));
    }
}

template<class TK>
void Parser<TK>::parseCreateTable() {
    expect(Token::CREATE);
    expect(Token::TABLE);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::FROM);
    expect(Token::FILE);
    string fileName = expect(Token::VALUE)->lexema;
    expect(Token::USING);
    expect(Token::INDEX);
    Token* indexType = expectOneOf({Token::AVL, Token::ISAM, Token::EXTENDIBLE});
    expect(Token::LPARENT);
    string indexField = expect(Token::VALUE)->lexema;
    expect(Token::RPARENT);

    Table table = {tableName, fileName, Token::token_names[indexType->type]};
    tables.push_back(table);
    if (indexType->type == Token::AVL) {
        if (tableName == "Apps") {
//            cout << "create table apps" << endl;
//            vector<Record<int>> records = readCSV_playstore("../" + fileName);
//            cout << "records size: " << records.size() << endl;
//            for (int i = 0; i < 5; ++i) {
//                avlApps->insert(records[i]);
//            }
            strcpy(fileStructure, "avlFileApps");
            avlApps->printAll();
        } else if (tableName == "Youtube") {
            vector<Record<const char*>> records = readCSV_youtube("../" + fileName);
            cout << "records size: " << records.size() << endl;
            cout << "create table youtube" << endl;
            strcpy(fileStructure, "avlFileYoutube");
        }
    }

    cout << "---- Created table " << tableName << " from file " << fileName << " using index " << indexField << " (" << Token::token_names[indexType->type] << ")" << endl;
}

template<class TK>
void Parser<TK>::parseSelect() {
    expect(Token::SELECT);
    expect(Token::ALL);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();

    if (tableName == "Apps") {
        if (condition.op == "=") {
//            Record<const char*> result = avlFileApp->find(condition.value1.c_str());
//            cout << "---- Selecting from " << tableName << " where " << condition.field << " = " << condition.value1 << ": " << result.toString() << endl;
        }
    } else if (tableName == "Animes") {
        if (condition.op == "=") {
//            cout << "---- Selecting from " << tableName << " where " << condition.field << " = " << condition.value1 << ": " << result.toString() << endl;
        } else if (condition.op == "between") {
            vector<Record<int>> results;
//            avlFileAnime->rangeSearch(stoi(condition.value1), stoi(condition.value2), results);
            for (Record<int>& rec : results) {
                rec.show();
            }
        }
    }
}

template<class TK>
void Parser<TK>::parseInsert() {
    expect(Token::INSERT);
    expect(Token::INTO);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::VALUES);
    expect(Token::LPARENT);
    vector<string> values = parseValues();
    expect(Token::RPARENT);

    if (tableName == "Youtube") {
        cout << "---- Inserting into " << tableName << " values: ";
        Record<int> record;
        strncpy(record.key, values[0].c_str(), sizeof(record.key) - 1);
        strncpy(record.App_name, values[1].c_str(), sizeof(record.App_name) - 1);
        strncpy(record.category, values[2].c_str(), sizeof(record.category) - 1);
        record.rating = stof(values[3]);
        record.rating_count = stof(values[4]);
        strncpy(record.installs, values[5].c_str(), sizeof(record.installs) - 1);
        record.minimum_installs = stof(values[6]);
        record.maximum_installs = stof(values[7]);
        record.free = values[8] == "true";
        record.price = stof(values[9]);
        strncpy(record.currency, values[10].c_str(), sizeof(record.currency) - 1);
        strncpy(record.size, values[11].c_str(), sizeof(record.size) - 1);
        avlApps->insert(record);

        cout << "---- Inserting into " << tableName << " values: ";
        for (const string& value : values) {
            cout << value << " ";
        }
        cout << endl;
    } else if (tableName == "Playstore") {
        Record<const char*> record;
        strncpy(record.key, values[0].c_str(), sizeof(record.key) - 1);
        strncpy(record.title, values[1].c_str(), sizeof(record.title) - 1);
        strncpy(record.channel_title, values[2].c_str(), sizeof(record.channel_title) - 1);
        record.views = stof(values[3]);
        record.likes = stof(values[4]);
        record.dislikes = stof(values[5]);
        record.comment_count = stof(values[6]);
        avlYoutube->insert(record);
    }

//    vector<Record<int>> sortedData = avlFileAnime->inorder();
//    for (Record<int>& rec : sortedData) {
//        rec.show();
//    }
}

template<class TK>
void Parser<TK>::parseDelete() {
    expect(Token::DELETE);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();

    if (tableName == "Apps") {
//        avlFileApp->remove(condition.value1.c_str());
        cout << "Deleting from " << tableName << " where " << condition.field << " = " << condition.value1;
    } else if (tableName == "Animes") {
//        avlFileAnime->remove(stoi(condition.value1));  // Assuming anime_id (integer) is key
//        cout << "Deleting from " << tableName << " where " << condition.field << " = " << condition.value1;
    }

    cout << endl;
}
template<class TK>
Condition Parser<TK>::parseCondition() {
    string field = expect(Token::ID)->lexema;
    Token* opToken = currentToken;
    if (opToken->type == Token::EQUAL) {
        expect(Token::EQUAL);
        string value = expect(Token::VALUE)->lexema;
        return {field, "=", value, ""};
    } else if (opToken->type == Token::BETWEEN) {
        expect(Token::BETWEEN);
        string value1 = expect(Token::VALUE)->lexema;
        expect(Token::AND);
        string value2 = expect(Token::VALUE)->lexema;
        return {field, "between", value1, value2};
    } else {
        error("Expected = or between");
        return {};
    }
}

template<class TK>
vector<string> Parser<TK>::parseValues() {
    vector<string> values;
    values.push_back(expect(Token::VALUE)->lexema);
    while (currentToken->type == Token::COLON) {
        expect(Token::COLON);
        values.push_back(expect(Token::VALUE)->lexema);
    }
    return values;
}


template<class TK>
Token* Parser<TK>::expect(Token::Type type) {
    if (currentToken->type == type) {
        Token* token = currentToken;
        currentToken = scanner->nextToken();
        return token;
    } else {
        error("Expected token: " + string(Token::token_names[type]) + " but got: " + string(Token::token_names[currentToken->type]));
        return nullptr;
    }
}


#endif //PROJECTO1_PARSERSQL_H
