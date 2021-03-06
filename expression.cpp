#include "expression.h"

#include <QDebug>
#include <QtMath>

Expression::Expression() {
}

const Expression* Expression::parse(const QString expression) {
    Tokenizer parser(expression);
    QList<const Token*>* tokens = parser.tokenize();
    qDebug() << parser.toString();
    QStack<const Token*> operators;
    QStack<const Expression*> operands;
    for (const Token* token : *tokens) {
        switch (token->type) {
        case OPERATOR: {
            if (token->content == ")") {
                bool hasLeftBracket = false;
                while (!operators.empty()) {
                    const Token* op = operators.pop();
                    if (op->content == "(") {
                        hasLeftBracket = true;
                        break;
                    }
                    const Expression *rhs = operands.empty() ? nullptr : operands.pop(),
                                     *lhs = operands.empty() ? nullptr : operands.pop();
                    if (!rhs || !lhs) {
                        throw SyntaxError(QString("Missing operands for operator ").arg(op->content));
                    }
                    operands.push(new CompoundExpression(op->content, lhs, rhs));
                }
                if (!hasLeftBracket) {
                    throw SyntaxError("Missing \"(\"");
                }
                break;
            } else if (token->content == "(") {
                operators.push(token);
            } else {
                while (!operators.empty() && operators.top()->precedence >= token->precedence) {
                    const Expression *rhs = operands.empty() ? nullptr : operands.pop(),
                                     *lhs = operands.empty() ? nullptr : operands.pop();
                    if (!rhs || !lhs) {
                        throw SyntaxError(QString("Missing operands for operator ").arg(token->content));
                    }
                    operands.push(new CompoundExpression(operators.pop()->content, lhs, rhs));
                }
                operators.push(token);
            }
            break;
        }
        case CONSTANT:
            operands.push(new ConstantExpression(token));
            break;
        case IDENTIFIER:
            operands.push(new IdentifierExpression(token));
            break;
        default:
            break;
        }
    }
    while (!operators.empty()) {
        const Token* op = operators.pop();
        const Expression *rhs = operands.empty() ? nullptr : operands.pop(),
                         *lhs = operands.empty() ? nullptr : operands.pop();
        if (!rhs || !lhs) {
            if (op->content == "(") {
                throw SyntaxError("Missing \")\"");
            }
            throw SyntaxError(QString("Missing operands for operator ").arg(op->content));
        }
        operands.push(new CompoundExpression(op->content, lhs, rhs));
    }
    if (operands.empty()) {
        throw SyntaxError("Missing expression");
    }
    return operands.pop();
}

int Expression::evaluate(Runtime&) const {
    return 0;
}

QString Expression::ast() const {
    return "";
}

QString Expression::getIdentifierName() const {
    return "";
}

QString Expression::toString() const {
    return "";
}

ExpressionType Expression::getType() const {
    return UNKOWN_EXP;
}

Expression::~Expression() {
}

// ConstantExpression
ConstantExpression::ConstantExpression(int val): value(val) {
}

ConstantExpression::ConstantExpression(const Token* token): value(token->content.toInt()) {
}

int ConstantExpression::evaluate(Runtime&) const {
    return value;
}

QString ConstantExpression::ast() const {
    return QString().setNum(value);
}

QString ConstantExpression::toString() const {
    return QString("ConstantExpression(%1)").arg(value);
}

ExpressionType ConstantExpression::getType() const {
    return CONST_EXP;
}

// IdentifierExpression
IdentifierExpression::IdentifierExpression(const QString i): identifier(i) {
}

IdentifierExpression::IdentifierExpression(const Token* token): identifier(token->content) {
}

int IdentifierExpression::evaluate(Runtime& context) const {
    return context.symbols.getValue(identifier);
}

QString IdentifierExpression::getIdentifierName() const {
    return identifier;
}

QString IdentifierExpression::ast() const {
    return identifier;
}

QString IdentifierExpression::toString() const {
    return QString("IdentifierExpression(%1)").arg(identifier);
}

ExpressionType IdentifierExpression::getType() const {
    return IDENTIFIER_EXP;
}

// CompoundExpression
CompoundExpression::CompoundExpression(const QString o, const Expression* l, const Expression* r)
    : op(o), lhs(l), rhs(r) {
}

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
    if (op == "**") {
        return qPow(left, right);
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
    return op + indent(QString("\n%1\n%2").arg(lhs->ast()).arg(rhs->ast()));
}

QString CompoundExpression::toString() const {
    return QString("CompoundExpression(%1, %2, %3)").arg(op).arg(lhs->toString()).arg(rhs->toString());
}

ExpressionType CompoundExpression::getType() const {
    return COMPOUND_EXP;
}

QString CompoundExpression::getOp() const {
    return op;
}

CompoundExpression::~CompoundExpression() {
    delete lhs;
    delete rhs;
}
