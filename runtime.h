#ifndef RUNTIME_H
#define RUNTIME_H

#include "evaluationcontext.h"
#include "pseudoio.h"

class Runtime {
  public:
    PseudoIO io;
    EvaluationContext symbols;
    QMap<int, QString>::const_iterator pc = nullptr;
    bool halt = false;
    Runtime();
    void reset();
};

#endif // RUNTIME_H
