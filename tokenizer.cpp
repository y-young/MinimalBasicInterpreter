#include "tokenizer.h"

#include <QDebug>

Token::Token(TokenType t, QString c): type(t), content(c) {
}

QString Token::toString() const {
    return QString("Token(%1, %2)").arg(type).arg(content);
}

Tokenizer::Tokenizer(const QString& exp): expression(exp) {
    tokens = new QList<const Token*>();
    currentTokenType = NONE;
    lastTokenType = NONE;
    currentToken.clear();
    lastToken.clear();
}

QList<const Token*>* Tokenizer::tokenize() {
    QChar c;
    int index = 0;
    while (index < expression.length()) {
        c = expression[index];
        if (c == ' ') {
            saveLastToken();
        } else if ((c >= '0' && c <= '9') || c == '-') {
            if (currentTokenType == NONE) {
                currentTokenType = NUMBER;
            }
            if (c != '-') {
                // n1, 123
                currentToken.append(c);
            } else if ((index + 1) < expression.length() && expression[index + 1] != ' ') { // 1- 1, 1 - 1
                saveLastToken();
                // is -1, 1-1; not - 1, 1 - 1, n - 1
                if (lastTokenType == NONE || lastToken == "(" || lastToken == ")" ||
                    lastToken == "=") { // -1, 1+(-1), a=-1
                    currentToken.append(c);
                } else {
                    handleOperator("-");
                }
            } else { // as operator
                handleOperator(c);
            }
        } else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
            if (currentTokenType == NUMBER) {
                throw SyntaxError("Invalid syntax " + currentToken + QString(c), expression);
            }
            currentTokenType = IDENTIFIER;
            currentToken.append(c);
        } else if (opList.contains(c)) {
            if (c == '*' && (index + 1) < expression.length() && expression[index + 1] == '*') {
                ++index;
                handleOperator("**");
            } else {
                handleOperator(c);
            }
        } else {
            throw SyntaxError("Invalid syntax " + QString(c), expression);
        }
        ++index;
    }
    saveLastToken();
    return tokens;
}

void Tokenizer::addToken(const Token* token) {
    tokens->push_back(token);
    lastTokenType = token->type;
    lastToken = token->content;
    currentTokenType = NONE;
    currentToken.clear();
}

void Tokenizer::saveLastToken() {
    if (currentTokenType != NONE && !currentToken.isEmpty()) {
        addToken(new Token(currentTokenType, currentToken));
    }
}

void Tokenizer::handleOperator(QString current) {
    saveLastToken();
    if (lastTokenType == OPERATOR && current != "(" && current != ")" && lastToken != "(" && lastToken != ")" &&
        lastToken != "=") {
        throw SyntaxError("Invalid syntax: " + lastToken + current, expression);
    }
    addToken(new Token(OPERATOR, current));
}

QString Tokenizer::toString() const {
    QString output = "";
    for (auto token : *tokens) {
        output += token->toString() + " ";
    }
    return output;
}

Tokenizer::~Tokenizer() {
    delete tokens;
}
