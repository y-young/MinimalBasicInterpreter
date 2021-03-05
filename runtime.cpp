#include "runtime.h"

Runtime::Runtime() {
}

void Runtime::reset() {
    symbols.clear();
    pc = nullptr;
    status = OK;
    gotoDst = 0;
}
