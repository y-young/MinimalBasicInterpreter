#ifndef RUNTIME_H
#define RUNTIME_H

#include "evaluationcontext.h"
#include "pseudoio.h"

enum StatusCode { OK, INTERRUPT, GOTO, HALT };

class Runtime {
  public:
    PseudoIO io;
    EvaluationContext symbols;
    QMap<int, QString>::const_iterator pc = nullptr;
    int gotoDst;
    StatusCode status = HALT;
    Runtime();
    void reset();
};

#endif // RUNTIME_H
