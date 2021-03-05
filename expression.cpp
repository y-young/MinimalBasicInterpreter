#include "expression.h"

Expression::Expression() {
}

int Expression::evaluate(Runtime& context) const {
    return 0;
}

QString Expression::ast() const {
    return "";
}

QString Expression::getIdentifierName() const {
    return "";
}

// ConstantExpression
int ConstantExpression::evaluate(Runtime&) const {
    return value;
}

QString ConstantExpression::ast() const {
    return QString().setNum(value);
}

// IdentifierExpression
int IdentifierExpression::evaluate(Runtime& context) const {
    return context.symbols.getValue(identifier);
}

QString IdentifierExpression::getIdentifierName() const {
    return identifier;
}

QString IdentifierExpression::ast() const {
    return "";
}

// CompoundExpression
int CompoundExpression::evaluate(Runtime& context) const {
    int right = rhs->evaluate(context);
    if (op == "=") {
        context.symbols.setValue(lhs->getIdentifierName(), right);
        return right;
    }
    int left = lhs->evaluate(context);
    if (op == "+") {
        return left + right;
    }
    if (op == "-") {
        return left - right;
    }
    if (op == "*") {
        return left * right;
    }
    if (op == "/") {
        if (right == 0) {
            throw RuntimeError("Division by 0");
        }
        return left / right;
    }
    throw SyntaxError("Illegal operator in expression");
    return 0;
}

QString CompoundExpression::ast() const {
    return "";
}
