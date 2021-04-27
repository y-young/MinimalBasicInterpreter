#include "runtime.h"

Runtime::Runtime(PseudoIO* _io): io(_io) {
}

void Runtime::reset() {
    symbols.clear();
    pc = nullptr;
    status = OK;
    gotoDst = 0;
}
