#ifndef STATEMENT_H
#define STATEMENT_H

#include <QDebug>
#include <QString>

#include "exceptions.h"
#include "runtime.h"

class Statement {
  public:
    Statement();
    static Statement* parse(QString statement);
    virtual void execute(Runtime& context) const;
    virtual const QString ast() const;
};

class RemarkStatement: public Statement {
  private:
    QString body;

  public:
    RemarkStatement(QString b): body(b) {
    }
    const QString ast() const override;
};

class LetStatement: public Statement {};

class PrintStatement: public Statement {
  private:
    // TODO: support expression
    QString identifier;

  public:
    PrintStatement(QString body);
    void execute(Runtime& context) const override;
};

class InputStatement: public Statement {
  private:
    QString identifier;

  public:
    InputStatement(QString body);
    void execute(Runtime& context) const override;
    const QString ast() const override;
};

class GotoStatement: public Statement {};

class IfStatement: public Statement {};

class EndStatement: public Statement {
  public:
    void execute(Runtime& context) const override;
    const QString ast() const override;
};

#endif // STATEMENT_H
