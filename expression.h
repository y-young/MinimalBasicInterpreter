#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>

#include "exceptions.h"
#include "runtime.h"

enum ExpressionType { CONSTANT, IDENTIFIER, COMPOUND };

class Expression {
  public:
    Expression();
    static Expression* parse(QString expression);
    virtual int evaluate(Runtime& context) const;
    virtual QString ast() const;
    virtual QString getIdentifierName() const;
};

class ConstantExpression: public Expression {
  private:
    int value;

  public:
    int evaluate(Runtime& context) const override;
    QString ast() const override;
};

class IdentifierExpression: public Expression {
  private:
    QString identifier;

  public:
    int evaluate(Runtime& context) const override;
    QString ast() const override;
    QString getIdentifierName() const override;
};

class CompoundExpression: public Expression {
  private:
    QString op;
    Expression *lhs, *rhs;

  public:
    int evaluate(Runtime& context) const override;
    QString ast() const override;
};

#endif // EXPRESSION_H
