#include "statement.h"

Statement::Statement() {
}

Statement::Statement(QString n): name(n) {
    error = new SyntaxError(QString("Unknown statement type \"%1\"").arg(name));
}

Statement* Statement::parse(const QString statement) {
    QString content = statement.trimmed();
    QString name = content.split(' ')[0];
    QString body = content.section(' ', 1);

    if (name == "REM") {
        return new RemarkStatement(body);
    } else if (name == "LET") {
        return new LetStatement(body);
    } else if (name == "PRINT") {
        return new PrintStatement(body);
    } else if (name == "PRINTF") {
        return new PrintfStatement(body);
    } else if (name == "INPUT") {
        return new InputStatement(body);
    } else if (name == "INPUTS") {
        return new InputsStatement(body);
    } else if (name == "GOTO") {
        return new GotoStatement(body);
    } else if (name == "IF") {
        return new IfStatement(body);
    } else if (name == "END") {
        return new EndStatement();
    }
    return new Statement(name);
}

void Statement::execute(Runtime*) const {
    throw error;
}

const QString Statement::ast() const {
    return "ERROR";
}

Statement::~Statement() {
}

// REM
const QString RemarkStatement::ast() const {
    return QString("REM\n    " + body);
}

void RemarkStatement::execute(Runtime*) const {
    return;
}

// LET
LetStatement::LetStatement(const QString body) {
    try {
        expression = Expression::parse(body);
    } catch (Exception* exception) {
        error = exception;
        return;
    }
    if (expression->error) {
        error = expression->error;
        return;
    }
    // ensure an assignment expression
    if (expression->getType() != COMPOUND_EXP || static_cast<const CompoundExpression*>(expression)->getOp() != "=") {
        error = new SyntaxError(QString("Invalid expression \"%1\" for LET statement").arg(body));
    }
}

void LetStatement::execute(Runtime* context) const {
    expression->evaluate(context);
}

const QString LetStatement::ast() const {
    if (error) {
        return "ERROR";
    }
    return "LET " + expression->ast();
}

LetStatement::~LetStatement() {
    delete expression;
}

// PRINT
PrintStatement::PrintStatement(const QString body) {
    try {
        expression = Expression::parse(body);
    } catch (Exception* exception) {
        error = exception;
        return;
    }
    error = expression->error;
}

void PrintStatement::execute(Runtime* context) const {
    const Value* value = expression->evaluate(context);
    context->io->output(value);
}

const QString PrintStatement::ast() const {
    if (error) {
        return "ERROR";
    }
    return "PRINT" + indent("\n" + expression->ast());
}

PrintStatement::~PrintStatement() {
    delete expression;
}

// PRINTF

PrintfStatement::PrintfStatement(const QString body) {
    QList<QString> arguments = getArguments(body);
    // Parse format string
    if (arguments.size() < 1) {
        error = new SyntaxError("Missing expression for PRINTF statement: " + body);
        return;
    }
    const Expression* exp = Expression::parse(arguments.first().trimmed());
    if (exp->getType() != CONST_EXP || !exp->ast().startsWith('"')) {
        error = new SyntaxError("Invalid format string for PRINTF statement: " + formatString);
        return;
    }
    formatString = exp->evaluate(nullptr)->toString();
    arguments.pop_front();
    // Parse arguments
    for (QString argument : arguments) {
        const Expression* arg;
        try {
            arg = Expression::parse(argument);
        } catch (Exception* exception) {
            error = exception;
            return;
        }
        error = arg->error;
        if (error) {
            return;
        }
        args.append(arg);
    }
    // Validate number of arguments
    int argCount = formatString.count(pattern);
    if (argCount != args.size()) {
        error = new SyntaxError(QString("Expecting %1 argument(s), found %2").arg(argCount).arg(args.size()));
        return;
    }
}

void PrintfStatement::execute(Runtime* context) const {
    QString formattedString = formatString;
    int index;
    for (const Expression* arg : args) {
        const Value* value = arg->evaluate(context);
        index = formattedString.indexOf(pattern);
        formattedString.replace(index, 2, value->toString());
    }
    context->io->output(formattedString);
}

const QString PrintfStatement::ast() const {
    if (error) {
        return "ERROR";
    }
    QString astString = "PRINTF" + indent("\n\"" + formatString + "\"");
    for (const Expression* arg : args) {
        astString += indent("\n" + arg->ast());
    }
    return astString;
}

QList<QString> PrintfStatement::getArguments(const QString body) const {
    QList<QString> arguments;
    bool inString = false;
    QString current;
    for (QChar c : body) {
        // Don't treat commas inside strings as arguments delimiter
        if (c == ',' && !inString) {
            arguments.append(current);
            current.clear();
        } else {
            current.append(c);
        }
        if (c == '"' || c == '\'') {
            if (inString) {
                inString = false;
            } else {
                inString = true;
            }
        }
    }
    if (!current.isEmpty()) {
        arguments.append(current);
    }
    return arguments;
}

PrintfStatement::~PrintfStatement() {
    for (const Expression* arg : args) {
        delete arg;
    }
}

// INPUT
InputStatement::InputStatement(const QString body): identifier(body) {
}

void InputStatement::execute(Runtime* context) const {
    // ensure an identifier expression
    const Expression* expression = Expression::parse(identifier);
    if (expression->getType() != IDENTIFIER_EXP) {
        throw new SyntaxError(QString("Invalid identifier \"%1\" for INPUT statement").arg(identifier));
    }
    context->io->requestInput(identifier);
    if (context->status == OK) { // Console input
        context->status = INTERRUPT;
    }
}

const QString InputStatement::ast() const {
    if (error) {
        return "ERROR";
    }
    return QString("INPUT\n    " + identifier);
}

// INPUTS
InputsStatement::InputsStatement(const QString body): InputStatement(body) {
}

void InputsStatement::execute(Runtime* context) const {
    InputStatement::execute(context);
    context->io->requestStringInput();
}

const QString InputsStatement::ast() const {
    if (error) {
        return "ERROR";
    }
    return QString("INPUTS\n    " + identifier);
}

// GOTO
GotoStatement::GotoStatement(const QString body): destination(body.trimmed()) {
    if (destination.isEmpty()) {
        error = new SyntaxError("Missing jump destination for GOTO statement");
    }
}

void GotoStatement::execute(Runtime* context) const {
    bool ok;
    int dest = destination.toInt(&ok);
    if (!ok) {
        throw new SyntaxError(QString("Invalid jump destination \"%1\"").arg(destination));
    }
    context->status = GOTO;
    context->gotoDst = dest;
}

QString GotoStatement::getDestination() const {
    return destination;
}

const QString GotoStatement::ast() const {
    if (error) {
        return "ERROR";
    }
    return QString("GOTO\n    %1").arg(destination);
}

// IF
IfStatement::IfStatement(const QString body) {
    int indexOfThen = body.indexOf("THEN");
    if (indexOfThen == -1) {
        error = new SyntaxError("Missing THEN clause for IF statement");
        return;
    }
    gotoStmt = new GotoStatement(body.mid(indexOfThen + 4));
    int indexOfOp = parseCondition(body);
    if (error) {
        return;
    }
    try {
        lhs = Expression::parse(body.left(indexOfOp));
        rhs = Expression::parse(body.mid(indexOfOp + 1, indexOfThen - indexOfOp - 1));
    } catch (Exception* exception) {
        error = exception;
        return;
    }
    error = lhs->error ? lhs->error : rhs->error;
}

void IfStatement::execute(Runtime* context) const {
    const Value &left = *lhs->evaluate(context), &right = *rhs->evaluate(context);
    bool shouldJump = (conditionOp == '<' && left < right) || (conditionOp == '=' && left == right) ||
                      (conditionOp == '>' && left > right);
    if (shouldJump) {
        gotoStmt->execute(context);
    }
}

const QString IfStatement::ast() const {
    if (error) {
        return "ERROR";
    }
    return QString("IF THEN\n    %1\n    %2\n    %3\n    %4")
        .arg(lhs->ast())
        .arg(conditionOp)
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
    error = new SyntaxError("Invalid condition operator for IF statement");
    return -1;
}

IfStatement::~IfStatement() {
    delete lhs;
    delete rhs;
    delete gotoStmt;
}

// END
void EndStatement::execute(Runtime* context) const {
    context->status = HALT;
}

const QString EndStatement::ast() const {
    return QString("END");
}
