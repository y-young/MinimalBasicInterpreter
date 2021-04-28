#ifndef EVALUATIONCONTEXT_H
#define EVALUATIONCONTEXT_H

#include <QMap>
#include <QString>

#include "exceptions.h"
#include "value.h"

class EvaluationContext {
  private:
    QMap<QString, const Value*> symbols;

  public:
    EvaluationContext();
    const Value* getValue(QString identifier) const;
    void setValue(QString identifier, const Value* value);
    void clear();
    QString toString() const;
};

#endif // EVALUATIONCONTEXT_H
