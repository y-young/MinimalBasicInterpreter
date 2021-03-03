#include "pseudoio.h"

#include <QDebug>

PseudoIO::PseudoIO() {
}

void PseudoIO::input(QString identifier) {
    emit requestInput(identifier);
}

void PseudoIO::input(QString identifier, QString stream) {
    int userInput;
    bool ok;
    userInput = stream.toInt(&ok);
    if (!ok) {
        // Invalid input, retry
        emit requestInput(identifier);
        return;
    }
    emit receivedInput(identifier, userInput);
}

void PseudoIO::output(int value) const {
    QString content;
    content.setNum(value);
    output(content);
}

void PseudoIO::output(QString content) const {
    emit printOutput(content);
}
