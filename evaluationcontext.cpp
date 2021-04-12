#include "evaluationcontext.h"

EvaluationContext::EvaluationContext() {
}

int EvaluationContext::getValue(QString identifier) const {
    QMap<QString, int>::const_iterator result;
    result = symbols.find(identifier);
    if (result == symbols.end()) {
        throw new RuntimeError("Undefined variable: " + identifier);
    }
    return result.value();
}

void EvaluationContext::setValue(QString identifier, int value) {
    symbols.insert(identifier, value);
}

void EvaluationContext::clear() {
    symbols.clear();
}
