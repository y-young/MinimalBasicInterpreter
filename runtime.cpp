#include "runtime.h"

Runtime::Runtime(PseudoIO* _io): io(_io) {
    symbols = new EvaluationContext();
    connect(symbols, &EvaluationContext::contextChanged, this, QOverload<>::of(&Runtime::refreshState));
}

Runtime::~Runtime() {
    delete symbols;
}

void Runtime::init() {
    pc = nullptr;
    status = OK;
    gotoDst = 0;
}

void Runtime::reset() {
    init();
    symbols->clear();
}

void Runtime::refreshState() {
    io->setState(symbols->toString());
}
