#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QChar>
#include <QList>
#include <QString>

#include "exceptions.h"

enum TokenType { NONE, IDENTIFIER, CONSTANT, OPERATOR };
const QList<QChar> opList{'+', '-', '*', '/', '(', ')', '='};

class Token {
  public:
    const TokenType type;
    const QString content;
    int precedence;
    Token(TokenType t, QString c);
    QString toString() const;
};

class Tokenizer {
  private:
    TokenType currentTokenType, lastTokenType;
    QString currentToken, lastToken;
    QList<const Token*>* tokens;
    const QString& expression;
    void addToken(const Token* token);
    void saveLastToken();
    void handleOperator(QString current);

  public:
    Tokenizer(const QString& exp);
    QList<const Token*>* tokenize();
    QString toString() const;
    ~Tokenizer();
};

#endif // TOKENIZER_H
