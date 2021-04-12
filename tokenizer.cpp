#include "tokenizer.h"

#include <QDebug>

Token::Token(TokenType t, QString c): type(t), content(c) {
    if (content == "**") {
        precedence = 4;
    } else if (content == "*" || content == "/") {
        precedence = 3;
    } else if (content == "+" || content == "-") {
        precedence = 2;
    } else {
        precedence = 1;
    }
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

QList<const Token*>* Tokenizer::tokenize(Exception** error) {
    QChar c;
    int index = 0;
    // remove spaces
    QString expression = this->expression;
    expression.replace(QString(' '), QString());
    try {
        while (index < expression.length()) {
            c = expression[index];
            if (c == ' ') {
                saveLastToken();
            } else if ((c >= '0' && c <= '9') || c == '-') {
                if (currentTokenType == NONE) {
                    currentTokenType = CONSTANT;
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
                if (currentTokenType == CONSTANT) {
                    *error = new SyntaxError(QString("Invalid syntax %1%2").arg(currentToken).arg(c));
                    return nullptr;
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
                *error = new SyntaxError("Invalid syntax " + QString(c));
                return nullptr;
            }
            ++index;
        }
    } catch (Exception* exception) {
        *error = exception;
        return nullptr;
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
        throw new SyntaxError(QString("Invalid syntax: %1%2").arg(lastToken).arg(current));
    }
    addToken(new Token(OPERATOR, current));
}

QString Tokenizer::toString() const {
    QString output = "";
    for (const Token* token : *tokens) {
        output += token->toString() + " ";
    }
    return output;
}

Tokenizer::~Tokenizer() {
    delete tokens;
}
