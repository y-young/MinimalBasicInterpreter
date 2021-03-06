#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QList>
#include <QStack>
#include <QString>

#include "exceptions.h"
#include "runtime.h"
#include "tokenizer.h"
#include "utils.h"

enum ExpressionType { UNKOWN_EXP, CONST_EXP, IDENTIFIER_EXP, COMPOUND_EXP };

class Expression {
  public:
    Expression();
    static const Expression* parse(const QString expression);
    virtual int evaluate(Runtime& context) const;
    virtual QString ast() const;
    virtual QString getIdentifierName() const;
    virtual QString toString() const;
    virtual ExpressionType getType() const;
    virtual ~Expression();
};

class ConstantExpression: public Expression {
  private:
    const int value;

  public:
    ConstantExpression(int val);
    ConstantExpression(const Token* token);
    int evaluate(Runtime& context) const override;
    QString ast() const override;
    QString toString() const override;
    ExpressionType getType() const override;
};

class IdentifierExpression: public Expression {
  private:
    const QString identifier;

  public:
    IdentifierExpression(const QString i);
    IdentifierExpression(const Token* token);
    int evaluate(Runtime& context) const override;
    QString ast() const override;
    QString getIdentifierName() const override;
    QString toString() const override;
    ExpressionType getType() const override;
};

class CompoundExpression: public Expression {
  private:
    const QString op;
    const Expression *lhs, *rhs;

  public:
    CompoundExpression(const QString o, const Expression* l, const Expression* r);
    int evaluate(Runtime& context) const override;
    QString ast() const override;
    QString toString() const override;
    ExpressionType getType() const override;
    QString getOp() const;
    ~CompoundExpression();
};

#endif // EXPRESSION_H
