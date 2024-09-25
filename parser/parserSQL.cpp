//
// Created by paolo on 17/09/2024.
//


#include "parserSQL.h"
#include "avlFile.h"
Parser::Parser(Scanner* scanner, AVLFile<const char*>* avlFileApp, AVLFile<int>* avlFileAnime)
        : scanner(scanner), avlFileApp(avlFileApp), avlFileAnime(avlFileAnime) {
    currentToken = scanner->nextToken();
}
void Parser::parse() {
    while (currentToken->type != Token::END) {
        parseStatement();
        if (currentToken->type == Token::SEMICOLON) {
            currentToken = scanner->nextToken();
        } else if (currentToken->type != Token::END) {
            error("Expected token: SEMICOLON but got: " + string(Token::token_names[currentToken->type]));
        }
    }
}


void Parser::parseStatement() {
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
void Parser::parseCreateTable() {
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
            avlFileApp->loadData(fileName);  // Load data into the app AVL tree
        } else if (tableName == "Animes") {
            avlFileAnime->loadData(fileName);  // Load data into the anime AVL tree
        }
    }

    cout << "---- Created table " << tableName << " from file " << fileName << " using index " << indexField << " (" << Token::token_names[indexType->type] << ")" << endl;
}

void Parser::parseSelect() {
    expect(Token::SELECT);
    expect(Token::ALL);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();

    if (tableName == "Apps") {
        if (condition.op == "=") {
            Record<const char*> result = avlFileApp->find(condition.value1.c_str());
            cout << "---- Selecting from " << tableName << " where " << condition.field << " = " << condition.value1 << ": " << result.toString() << endl;
        }
    } else if (tableName == "Animes") {
        if (condition.op == "=") {
            Record<int> result = avlFileAnime->find(stoi(condition.value1));
            cout << "---- Selecting from " << tableName << " where " << condition.field << " = " << condition.value1 << ": " << result.toString() << endl;
        } else if (condition.op == "between") {
            vector<Record<int>> results;
            avlFileAnime->rangeSearch(stoi(condition.value1), stoi(condition.value2), results);
            for (Record<int>& rec : results) {
                rec.show();
            }
        }
    }
}

void Parser::parseInsert() {
    expect(Token::INSERT);
    expect(Token::INTO);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::VALUES);
    expect(Token::LPARENT);
    vector<string> values = parseValues();
    expect(Token::RPARENT);

    if (tableName == "Apps") {
        Record<const char*> record;
        strncpy(record.App_name, values[0].c_str(), sizeof(record.App_name) - 1);
        strncpy(record.App_id, values[1].c_str(), sizeof(record.App_id) - 1);
        strncpy(record.category, values[2].c_str(), sizeof(record.category) - 1);
        record.rating = stof(values[3]);
        record.rating_count = stof(values[4]);
        strncpy(record.installs, values[5].c_str(), sizeof(record.installs) - 1);
        record.minimum_installs = stof(values[6]);
        record.maximum_installs = stof(values[7]);
        record.free = (values[8] == "True");
        record.price = stof(values[9]);
        strncpy(record.currency, values[10].c_str(), sizeof(record.currency) - 1);
        strncpy(record.size, values[11].c_str(), sizeof(record.size) - 1);
        avlFileApp->insert(record);

        cout << "---- Inserting into " << tableName << " values: ";
        for (const string& value : values) {
            cout << value << " ";
        }
        cout << endl;

    } else if (tableName == "Animes") {
        Record<int> record;
        record.anime_id = stoi(values[0]);
        strncpy(record.name, values[1].c_str(), sizeof(record.name) - 1);
        strncpy(record.genre, values[2].c_str(), sizeof(record.genre) - 1);
        strncpy(record.type, values[3].c_str(), sizeof(record.type) - 1);
        record.episodes = stoi(values[4]);
        record.key = stof(values[5]);  // Assuming 'key' refers to some rating or score
        record.members = stoi(values[6]);
        avlFileAnime->insert(record);

        cout << "---- Inserting into " << tableName << " values: ";
        for (const string& value : values) {
            cout << value << " ";
        }
        cout << endl;
    }

    vector<Record<int>> sortedData = avlFileAnime->inorder();
    for (Record<int>& rec : sortedData) {
        rec.show();
    }
}

void Parser::parseDelete() {
    expect(Token::DELETE);
    expect(Token::FROM);
    string tableName = expect(Token::ID)->lexema;
    expect(Token::WHERE);
    Condition condition = parseCondition();

    if (tableName == "Apps") {
        avlFileApp->remove(condition.value1.c_str());
        cout << "Deleting from " << tableName << " where " << condition.field << " = " << condition.value1;
    } else if (tableName == "Animes") {
        avlFileAnime->remove(stoi(condition.value1));  // Assuming anime_id (integer) is key
        cout << "Deleting from " << tableName << " where " << condition.field << " = " << condition.value1;
    }

    cout << endl;
}

Condition Parser::parseCondition() {
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


vector<string> Parser::parseValues() {
    vector<string> values;
    values.push_back(expect(Token::VALUE)->lexema);
    while (currentToken->type == Token::COLON) {
        expect(Token::COLON);
        values.push_back(expect(Token::VALUE)->lexema);
    }
    return values;
}

Token* Parser::expect(Token::Type type) {
    if (currentToken->type == type) {
        Token* token = currentToken;
        currentToken = scanner->nextToken();
        return token;
    } else {
        error("Expected token: " + string(Token::token_names[type]) + " but got: " + string(Token::token_names[currentToken->type]));
        return nullptr;
    }
}
