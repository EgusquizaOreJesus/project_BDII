// Created by paolo on 17/09/2024.
//

#ifndef PROJECTO1_PARSERSQL_H
#define PROJECTO1_PARSERSQL_H

#include <iostream>
#include <vector>
#include "memory"
#include <string>
#include "tokenS.h"
#include "estructuras/avlFile.h"
#include "estructuras/ExtendibleHashing.h"
#include "estructuras/Sequential.h"

using namespace std;

struct Table
{
    string name;
    string file;
    string index;
};

struct Condition
{
    string field;
    string op;
    string value1;
    string value2;
};

class DataStructureFactory
{
public:
    template <typename TK>
    static std::unique_ptr<FileStructure<TK>> createAVL(string filename)
    {
        return std::make_unique<AVLFile<TK>>(filename);
    }

    template <typename TK>
    static std::unique_ptr<FileStructure<TK>> createExtendible(string filename, string directory)
    {
        return std::make_unique<ExtendibleHashing<TK>>(filename, directory);
    }

    template <typename TK>
    static std::unique_ptr<FileStructure<TK>> createSequential(string filename)
    {
        return std::make_unique<Sequential<TK>>(filename);
    }

    // AGREGAR MAS ESTRUCTURAS
};
template <class TK>
class Parser
{
public:
    Parser(Scanner *scanner, const char *estructura);
    Parser(Scanner *scanner);
    void parse();
    char fileStructure[20];
    vector<Record<TK>> records;

    const char *getStructure()
    {
        return fileStructure;
    }
    Record<TK> readRecord(vector<string> atributes);
    vector<Record<TK>> getRecords();
    ~Parser()
    {
    }
    // vector<Record<TK>> records;
private:
    std::unique_ptr<FileStructure<TK>> instance;

    Scanner *scanner;
    Token *currentToken;
    vector<Table> tables;
    void parseStatement();
    void parseCreateTable();
    void parseSelect();
    void parseInsert();
    void parseDelete();
    Condition parseCondition();
    vector<string> parseValues();
    Token *expect(Token::Type type);
    Token *expectOneOf(const std::initializer_list<Token::Type> &types);
    void error(const string &message);
};

template <class TK>
Record<TK> Parser<TK>::readRecord(vector<string> atributes)
{
    return Record<TK>();
}

template <class TK>
vector<Record<TK>> Parser<TK>::getRecords()
{
    return records;
}

template <>
Record<int> Parser<int>::readRecord(vector<string> values)
{
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
    return record;
}
template <>
Record<const char *> Parser<const char *>::readRecord(vector<string> values)
{
    Record<const char *> record;
    strncpy(record.key, values[0].c_str(), sizeof(record.key) - 1);
    strncpy(record.title, values[1].c_str(), sizeof(record.title) - 1);
    strncpy(record.channel_title, values[2].c_str(), sizeof(record.channel_title) - 1);
    record.views = stof(values[3]);
    record.likes = stof(values[4]);
    record.dislikes = stof(values[5]);
    record.comment_count = stof(values[6]);
    return record;
}
template <class TK>
Parser<TK>::Parser(Scanner *scanner)
{
    this->scanner = scanner;
    //    avlApps = new AVLFile<int>("playstore.dat");
    //    avlYoutube = new AVLFile<const char*>("youtube_data.bin");
    currentToken = scanner->nextToken();
}

template <class TK>
Parser<TK>::Parser(Scanner *scanner, const char *estructura)
{
    this->scanner = scanner;
    if (strcmp(estructura, "avlFilePlaystore") == 0)
    {
        this->instance = DataStructureFactory::createAVL<TK>("playstore1.dat");
        strcpy(fileStructure, "avlFilePlaystore");
    }
    else if (strcmp(estructura, "avlFileYoutube") == 0)
    {
        this->instance = DataStructureFactory::createAVL<TK>("youtube1.dat");
        strcpy(fileStructure, "avlFileYoutube");
    }
    else if (strcmp(estructura, "extendibleFileYoutube") == 0)
    {
        cout << "extendibleFileYoutube" << endl;
        this->instance = DataStructureFactory::createExtendible<TK>("youtube2.dat", "dir1.dat");
        strcpy(fileStructure, "extendibleFileYoutube");
    }
    else if (strcmp(estructura, "extendibleFilePlaystore") == 0)
    {
        this->instance = DataStructureFactory::createExtendible<TK>("playstore2.dat", "dir2.dat");
        strcpy(fileStructure, "extendibleFilePlaystore");
    }
    else if (strcmp(estructura, "sequentialFileYoutube") == 0)
    {
        cout << "sequentialFileYoutube" << endl;
        this->instance = DataStructureFactory::createSequential<TK>("youtube3.dat");
        cout << "instance created" << endl;
        strcpy(fileStructure, "sequentialFileYoutube");
    }
    else if (strcmp(estructura, "sequentialFilePlaystore") == 0)
    {
        this->instance = DataStructureFactory::createSequential<TK>("playstore3.dat");
        strcpy(fileStructure, "sequentialFilePlaystore");
    }
    currentToken = scanner->nextToken();
}

template <class TK>
Token *Parser<TK>::expectOneOf(const initializer_list<Token::Type> &types)
{
    cout << "---- Expecting one of the following tokens" << endl;
    cout << "current token: " << currentToken->type << endl;
    for (Token::Type type : types)
    {
        cout << "type: " << type << endl;
        if (currentToken->type == type)
        {
            cout << "found token: " << Token::token_names[type] << endl;
            Token *token = currentToken;
            currentToken = scanner->nextToken();
            return token;
        }
    }
    error("Expected one of the following tokens: ");
    for (Token::Type type : types)
    {
        error(string(Token::token_names[type]) + " ");
    }
    return nullptr;
}
template <class TK>
void Parser<TK>::error(const string &message)
{
}

template <class TK>
void Parser<TK>::parse()
{
    while (currentToken->type != Token::END)
    {
        parseStatement();
        if (currentToken->type == Token::SEMICOLON)
        {
            currentToken = scanner->nextToken();
        }
        else if (currentToken->type != Token::END)
        {
            error("Expected token: SEMICOLON but got: " + string(Token::token_names[currentToken->type]));
        }
    }
}

template <class TK>
void Parser<TK>::parseStatement()
{
    if (currentToken->type == Token::CREATE)
    {
        parseCreateTable();
    }
    else if (currentToken->type == Token::SELECT)
    {
        parseSelect();
    }
    else if (currentToken->type == Token::INSERT)
    {
        parseInsert();
    }
    else if (currentToken->type == Token::DELETE)
    {
        parseDelete();
    }
    else
    {
        error("Unexpected token: " + string(Token::token_names[currentToken->type]));
    }
}

template <class TK>
void Parser<TK>::parseCreateTable()
{
    cout << "---- Parsing CREATE TABLE" << endl;
    expect(Token::CREATE);
    expect(Token::TABLE);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::FROM);
    expect(Token::FILE);
    string fileName = expect(Token::VALUE)->lexema;
    expect(Token::USING);
    expect(Token::INDEX);
    cout << "tableName: " << tableName << endl;
    Token *indexType = expectOneOf({Token::AVL, Token::ISAM, Token::EXTENDIBLE, Token::SEQUENTIAL});
    cout << "indexType: " << Token::token_names[indexType->type] << endl;
    expect(Token::LPARENT);
    string indexField = expect(Token::VALUE)->lexema;
    expect(Token::RPARENT);
    cout << "filename: " << fileName << endl;

    Table table = {tableName, fileName, Token::token_names[indexType->type]};
    tables.push_back(table);
    vector<Record<TK>> records = readCSV<TK>(fileName);
    this->records = records;
    cout << "records size: " << records.size() << endl;
    if (indexType->type == Token::AVL)
    {
        if (tableName == "Playstore")
        {
            for (int i = 0; i < 5000; ++i)
            {
                instance->insert(records[i]);
            }
            strcpy(fileStructure, "avlFilePlaystore");
            instance->printAll();
        }
        else if (tableName == "Youtube")
        {
            cout << "inserting youtube" << endl;
            //            cout << records[0].key << endl;
            for (int i = 0; i < 5000; i++)
            {
                instance->insert(records[i]);
            }
            //            instance->printAll();
            strcpy(fileStructure, "avlFileYoutube");
        }
    }
    else if (indexType->type == Token::EXTENDIBLE)
    {
        if (tableName == "Playstore")
        {
            for (int i = 0; i < 100000; ++i)
            {
                instance->insert(records[i]);
            }
            // flush disk
            instance->update_disk();
            strcpy(fileStructure, "extendibleFilePlaystore");
        }
        else if (tableName == "Youtube")
        {
            for (int i = 0; i < records.size(); i++)
            {
                instance->insert(records[i]);
            }
            // flush disk
            instance->update_disk();
            strcpy(fileStructure, "extendibleFileYoutube");
        }
    }
    else if (indexType->type == Token::SEQUENTIAL)
    {
        if (tableName == "Playstore")
        {
            this->instance->buildCSV(records);
            strcpy(fileStructure, "sequentialFilePlaystore");
        }
        else if (tableName == "Youtube")
        {
            this->instance->buildCSV(records);
            strcpy(fileStructure, "sequentialFileYoutube");
        }
    }
    cout << "---- Created table " << tableName << " from file " << fileName << " using index " << indexField << " (" << Token::token_names[indexType->type] << ")" << endl;
}

template <class TK>
void Parser<TK>::parseSelect()
{
    expect(Token::SELECT);
    expect(Token::ALL);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();

    if (tableName == "Playstore")
    {
        if (condition.op == "=")
        {
            const char *key = condition.value1.c_str();
            Record<TK> result = instance->search(key);
            if (strlen(result.key) > 0)
            {
                records.push_back(result);
                std::cout << result.show() << std::endl;
            }
            cout << result.show() << endl;
        }
        else if (condition.op == "between")
        {
            vector<Record<TK>> results = instance->range_search(condition.value1.c_str(), condition.value2.c_str());
            this->records = results;
        }
    }
    else if (tableName == "Youtube")
    {
        cout << "Searching in Youtube" << endl;
        if (condition.op == "=")
        {
            const char *key = condition.value1.c_str();
            cout << "Searching for key: " << key << endl;
            Record<TK> result = instance->search(key);
            cout << "finished search" << endl;
            if (strlen(result.key) > 0)
            {
                records.push_back(result);
                std::cout << result.show() << std::endl;
            }
            cout << result.show() << endl;
        }
        else if (condition.op == "between")
        {
            vector<Record<TK>> results = instance->range_search(condition.value1.c_str(), condition.value2.c_str());
            this->records = results;
            cout << "Results size: " << results.size() << endl;
        }
    }
}

template <class TK>
void Parser<TK>::parseInsert()
{
    expect(Token::INSERT);
    expect(Token::INTO);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::VALUES);
    expect(Token::LPARENT);
    vector<string> values = parseValues();
    expect(Token::RPARENT);

    if (tableName == "Youtube")
    {
        Record<TK> record = readRecord(values);
        this->instance->insert(record);
        cout << endl;
    }
    else if (tableName == "Playstore")
    {
        Record<TK> record = readRecord(values);
        this->instance->insert(record);
    }
}

template <class TK>
void Parser<TK>::parseDelete()
{
    expect(Token::DELETE);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();

    if (tableName == "Youtube")
    {
        if (condition.op == "=")
        {
            const char *key = condition.value1.c_str();
            instance->remove(key);
        }
    }
    else if (tableName == "Playstore")
    {
        if (condition.op == "=")
        {
            const char *key = condition.value1.c_str();
            instance->remove(key);
        }
    }

    cout << endl;
}
template <class TK>
Condition Parser<TK>::parseCondition()
{
    auto token = expect(Token::ID);
    if (token == nullptr)
    {
        cout << "---- Error parsing condition" << endl;
        return {};
    }
    string field = token->lexema;
    Token *opToken = currentToken;

    if (opToken->type == Token::EQUAL)
    {
        expect(Token::EQUAL);
        string value = expect(Token::VALUE)->lexema;
        return {field, "=", value, ""};
    }
    else if (opToken->type == Token::BETWEEN)
    {
        expect(Token::BETWEEN);
        string value1 = expect(Token::VALUE)->lexema;
        expect(Token::AND);
        string value2 = expect(Token::VALUE)->lexema;
        return {field, "between", value1, value2};
    }
    else
    {
        error("Expected = or between");
        return {};
    }
}

template <class TK>
vector<string> Parser<TK>::parseValues()
{
    vector<string> values;
    auto value = expect(Token::VALUE)->lexema;
    values.push_back(value);
    while (currentToken->type == Token::COLON)
    {
        expect(Token::COLON);
        if (currentToken->type == Token::VALUE)
        {
            values.push_back(expect(Token::VALUE)->lexema);
        }
        else if (currentToken->type == Token::NUM)
        {
            values.push_back(expect(Token::NUM)->lexema);
        }
        else if (currentToken->type == Token::FLOAT)
        {
            values.push_back(expect(Token::FLOAT)->lexema);
        }
        else if (currentToken->type == Token::TRUE)
        {
            values.push_back(expect(Token::TRUE)->lexema);
        }
        else if (currentToken->type == Token::FALSE)
        {
            values.push_back(expect(Token::FALSE)->lexema);
        }
    }
    return values;
}

template <class TK>
Token *Parser<TK>::expect(Token::Type type)
{
    if (currentToken->type == type)
    {
        Token *token = currentToken;
        currentToken = scanner->nextToken();
        return token;
    }
    else
    {
        error("Expected token: " + string(Token::token_names[type]) + " but got: " + string(Token::token_names[currentToken->type]));
        return nullptr;
    }
}

#endif // PROJECTO1_PARSERSQL_H
