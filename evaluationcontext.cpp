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

QString EvaluationContext::toString() const {
    QString content;
    QMap<QString, int>::const_iterator i;
    for (i = symbols.constBegin(); i != symbols.constEnd(); ++i) {
        content += QString("%1: INT = %2\n").arg(i.key()).arg(i.value());
    }
    return content;
}
