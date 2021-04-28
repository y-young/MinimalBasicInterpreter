#ifndef STATEMENT_H
#define STATEMENT_H

#include <QDebug>
#include <QString>

#include "exceptions.h"
#include "expression.h"
#include "runtime.h"
#include "utils.h"

class Statement {
  private:
    QString name;

  public:
    Exception* error = nullptr;
    Statement();
    Statement(QString n);
    static Statement* parse(const QString statement);
    virtual void execute(Runtime* context) const;
    virtual const QString ast() const;
    virtual ~Statement();
};

class RemarkStatement: public Statement {
  private:
    QString body;

  public:
    RemarkStatement(const QString b): body(b) {
    }
    void execute(Runtime* context) const override;
    const QString ast() const override;
};

class LetStatement: public Statement {
  private:
    const Expression* expression;

  public:
    LetStatement(const QString body);
    void execute(Runtime* context) const override;
    const QString ast() const override;
    ~LetStatement();
};

class PrintStatement: public Statement {
  private:
    const Expression* expression;

  public:
    PrintStatement(const QString body);
    void execute(Runtime* context) const override;
    const QString ast() const override;
    ~PrintStatement();
};

class PrintfStatement: public Statement {
  private:
    QString formatString;
    QList<const Expression*> args;
    const QString pattern = "{}";

  public:
    PrintfStatement(const QString body);
    void execute(Runtime* context) const override;
    const QString ast() const override;
    ~PrintfStatement();
};

class InputStatement: public Statement {
  protected:
    QString identifier;

  public:
    InputStatement(const QString body);
    void execute(Runtime* context) const override;
    const QString ast() const override;
};

class InputsStatement: public InputStatement {
  public:
    InputsStatement(const QString body);
    void execute(Runtime* context) const override;
    const QString ast() const override;
};

class GotoStatement: public Statement {
  private:
    const QString destination;

  public:
    GotoStatement(const QString body);
    void execute(Runtime* context) const override;
    const QString ast() const override;
    QString getDestination() const;
};

class IfStatement: public Statement {
  private:
    QChar conditionOp;
    const Expression *lhs, *rhs;
    const GotoStatement* gotoStmt;
    // Parse condition, set condition operator, and return index of operator
    int parseCondition(const QString condition);

  public:
    IfStatement(const QString body);
    void execute(Runtime* context) const override;
    const QString ast() const override;
    ~IfStatement();
};

class EndStatement: public Statement {
  public:
    void execute(Runtime* context) const override;
    const QString ast() const override;
};

#endif // STATEMENT_H
