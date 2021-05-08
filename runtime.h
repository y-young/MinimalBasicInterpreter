#ifndef RUNTIME_H
#define RUNTIME_H

#include <QObject>

#include "evaluationcontext.h"
#include "pseudoio.h"

enum StatusCode { OK, INTERRUPT, GOTO, HALT };

class Runtime: public QObject {
    Q_OBJECT
  public:
    PseudoIO* io;
    EvaluationContext* symbols;
    QMap<int, QString>::const_iterator pc = nullptr;
    int gotoDst;
    StatusCode status = HALT;
    Runtime(PseudoIO* _io);
    void init();
    void reset();
    void refreshState();
    ~Runtime();
};

#endif // RUNTIME_H
