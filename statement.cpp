#include "statement.h"

Statement::Statement() {
}

Statement* Statement::parse(const QString statement) {
    QString name = statement.split(' ')[0];
    QString body = statement.section(' ', 1);

    if (name == "REM") {
        return new RemarkStatement(body);
    } else if (name == "LET") {
        return new LetStatement(body);
    } else if (name == "PRINT") {
        return new PrintStatement(body);
    } else if (name == "INPUT") {
        return new InputStatement(body);
    } else if (name == "GOTO") {
        return new GotoStatement(body);
    } else if (name == "IF") {
        return new IfStatement(body);
    } else if (name == "END") {
        return new EndStatement();
    }
    throw SyntaxError(QString("Unknown statement type \"%1\"").arg(name));
}

void Statement::execute(Runtime&) const {
}

const QString Statement::ast() const {
    return "";
}

Statement::~Statement() {
}

// REM
const QString RemarkStatement::ast() const {
    return QString("REM\n    " + body);
}

// LET
LetStatement::LetStatement(const QString body) {
    expression = Expression::parse(body);
    // ensure an assignment expression
    if (expression->getType() != COMPOUND_EXP || static_cast<const CompoundExpression*>(expression)->getOp() != "=") {
        throw SyntaxError(QString("Invalid expression \"%1\" for LET statement").arg(body));
    }
}

void LetStatement::execute(Runtime& context) const {
    expression->evaluate(context);
}

const QString LetStatement::ast() const {
    return "LET " + expression->ast(); //.replace('\n', "\n    ");
}

LetStatement::~LetStatement() {
    delete expression;
}

// PRINT
PrintStatement::PrintStatement(const QString body) {
    expression = Expression::parse(body);
}

void PrintStatement::execute(Runtime& context) const {
    int value = expression->evaluate(context);
    context.io.output(value);
}

const QString PrintStatement::ast() const {
    return "PRINT" + indent("\n" + expression->ast());
}

PrintStatement::~PrintStatement() {
    delete expression;
}

// INPUT
InputStatement::InputStatement(const QString body): identifier(body) {
    // ensure an identifier expression
    const Expression* expression = Expression::parse(identifier);
    if (expression->getType() != IDENTIFIER_EXP) {
        throw SyntaxError(QString("Invalid idenitfier \"%1\" for INPUT statement").arg(body));
    }
}

void InputStatement::execute(Runtime& context) const {
    context.io.input(identifier);
    context.status = INTERRUPT;
}

const QString InputStatement::ast() const {
    return QString("INPUT\n    " + identifier);
}

// GOTO
GotoStatement::GotoStatement(const QString body) {
    bool ok;
    destination = body.toInt(&ok);
    if (!ok) {
        throw SyntaxError(QString("Invalid jump destination \"%1\"").arg(body));
    }
}

void GotoStatement::execute(Runtime& context) const {
    context.status = GOTO;
    context.gotoDst = destination;
}

int GotoStatement::getDestination() const {
    return destination;
}

const QString GotoStatement::ast() const {
    return QString("GOTO\n    %1").arg(destination);
}

// IF
IfStatement::IfStatement(const QString body) {
    int indexOfThen = body.indexOf("THEN");
    if (indexOfThen == -1) {
        throw SyntaxError("Missing THEN clause for IF statement");
    }
    gotoStmt = new GotoStatement(body.mid(indexOfThen + 4));
    int indexOfOp = parseCondition(body);
    lhs = Expression::parse(body.left(indexOfOp));
    rhs = Expression::parse(body.mid(indexOfOp + 1, indexOfThen - indexOfOp - 1));
}

void IfStatement::execute(Runtime& context) const {
    int left = lhs->evaluate(context), right = rhs->evaluate(context);
    bool shouldJump = (conditionOp == '<' && left < right) || (conditionOp == '=' && left == right) ||
                      (conditionOp == '>' && left > right);
    if (shouldJump) {
        gotoStmt->execute(context);
    }
}

const QString IfStatement::ast() const {
    return QString("IF THEN\n    %1\n    %2\n    %3\n    %4")
        .arg(conditionOp)
        .arg(lhs->ast())
        .arg(rhs->ast())
        .arg(gotoStmt->getDestination());
}

int IfStatement::parseCondition(const QString condition) {
    int index;
    index = condition.indexOf('<');
    if (index != -1) {
        conditionOp = '<';
        return index;
    }
    index = condition.indexOf('=');
    if (index != -1) {
        conditionOp = '=';
        return index;
    }
    index = condition.indexOf('>');
    if (index != -1) {
        conditionOp = '>';
        return index;
    }
    throw SyntaxError("Invalid condition operator for IF statement");
}

IfStatement::~IfStatement() {
    delete lhs;
    delete rhs;
    delete gotoStmt;
}

// END
void EndStatement::execute(Runtime& context) const {
    context.status = HALT;
}

const QString EndStatement::ast() const {
    return QString("END");
}
