#include "evaluationcontext.h"

EvaluationContext::EvaluationContext() {
}

EvaluationContext::~EvaluationContext() {
}

const Value* EvaluationContext::getValue(QString identifier) const {
    QMap<QString, const Value*>::const_iterator result;
    result = symbols.find(identifier);
    if (result == symbols.end()) {
        throw new RuntimeError("Undefined variable: " + identifier);
    }
    return result.value();
}

void EvaluationContext::setValue(QString identifier, const Value* value) {
    symbols.insert(identifier, value);
    emit contextChanged();
}

void EvaluationContext::clear() {
    symbols.clear();
}

QString EvaluationContext::toString() const {
    QString content;
    QMap<QString, const Value*>::const_iterator i;
    for (i = symbols.constBegin(); i != symbols.constEnd(); ++i) {
        const Value* value = i.value();
        content += QString("%1: %2 = %3\n").arg(i.key()).arg(value->type()).arg(value->ast());
    }
    return content;
}
