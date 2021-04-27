#include "pseudoio.h"

#include <QDebug>

PseudoIO::PseudoIO(QTextBrowser* code, QTextBrowser* output, QTextBrowser* ast)
    : codeDisplay(code), outputDisplay(output), astDisplay(ast) {
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
    outputDisplay->append(content);
}

void PseudoIO::setCode(QString code) const {
    codeDisplay->setText(code);
}

void PseudoIO::setAst(QString ast) const {
    astDisplay->setText(ast);
}

void PseudoIO::clear() const {
    codeDisplay->clear();
    outputDisplay->clear();
    astDisplay->clear();
}

void PseudoIO::clearOutput() const {
    outputDisplay->clear();
}

void PseudoIO::clearAst() const {
    astDisplay->clear();
}
