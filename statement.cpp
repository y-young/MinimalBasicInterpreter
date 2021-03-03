#include "statement.h"

Statement::Statement() {
}

Statement* Statement::parse(QString statement) {
    QString name = statement.split(' ')[0];
    QString body = statement.section(' ', 1);

    if (name == "REM") {
        return new RemarkStatement(body);
    } else if (name == "LET") {
        return new LetStatement();
    } else if (name == "PRINT") {
        return new PrintStatement(body);
    } else if (name == "INPUT") {
        return new InputStatement(body);
    } else if (name == "GOTO") {
        return new GotoStatement();
    } else if (name == "IF") {
        return new IfStatement();
    } else if (name == "END") {
        return new EndStatement();
    }
    throw SyntaxError("Unknown statement type: " + name, statement);
}

void Statement::execute(Runtime&) const {
}

const QString Statement::ast() const {
    return "";
}

// REM
const QString RemarkStatement::ast() const {
    return QString("REM\n    " + body);
}

// PRINT
PrintStatement::PrintStatement(QString body): identifier(body) {
}

void PrintStatement::execute(Runtime& context) const {
    int value = context.symbols.getValue(identifier);
    context.io.output(value);
}

// INPUT
InputStatement::InputStatement(QString body): identifier(body) {
}

void InputStatement::execute(Runtime& context) const {
    context.io.input(identifier);
    context.halt = true;
}

const QString InputStatement::ast() const {
    return QString("INPUT\n    " + identifier);
}

// END
void EndStatement::execute(Runtime& context) const {
    context.halt = true;
}

const QString EndStatement::ast() const {
    return QString("END");
}
