#ifndef EVALUATIONCONTEXT_H
#define EVALUATIONCONTEXT_H

#include <QMap>
#include <QString>

#include "exceptions.h"

class EvaluationContext {
  private:
    QMap<QString, int> symbols;

  public:
    EvaluationContext();
    int getValue(QString identifier) const;
    void setValue(QString identifier, int value);
    void clear();
};

#endif // EVALUATIONCONTEXT_H
