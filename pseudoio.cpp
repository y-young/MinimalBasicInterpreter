#include "pseudoio.h"

#include <QDebug>

PseudoIO::PseudoIO(QTextBrowser* code, QTextBrowser* output, QTextBrowser* ast, QLineEdit* command)
    : codeDisplay(code), outputDisplay(output), astDisplay(ast), commandInput(command) {
    connect(commandInput, &QLineEdit::returnPressed, this, QOverload<>::of(&PseudoIO::handleInput));
    commandInput->setFocus();
}

// Program requested input
void PseudoIO::requestInput(QString identifier) {
    commandInput->setText("? ");
    commandInput->setFocus();
    awaitingInput = true;
    awaitingIdentifier = identifier;
}

// User pressed enter in command input box
void PseudoIO::handleInput() {
    QString stream = commandInput->text();
    if (stream.isEmpty()) {
        return;
    }
    if (!awaitingInput) { // Treat as command
        emit receivedCommand(stream);
        return;
    }
    // Handle input requested by program
    if (!stream.startsWith("? ")) {
        commandInput->setText("? ");
        return;
    }
    commandInput->clear();
    stream = stream.section(' ', 1); // Get actual input value
    finishInput(stream);
}

// Pass input back to program
void PseudoIO::finishInput(QString stream) {
    int userInput;
    bool ok;
    userInput = stream.toInt(&ok);
    if (!ok) {
        // Invalid input, retry
        requestInput(awaitingIdentifier);
        return;
    }
    // Emit signal to program
    emit receivedInput(awaitingIdentifier, userInput);
    awaitingInput = false;
}

// Program produced output
void PseudoIO::output(int value) const {
    QString content;
    content.setNum(value);
    output(content);
}

void PseudoIO::output(QString content) const {
    outputDisplay->append(content);
}

void PseudoIO::setCode(QString code) const {
    codeDisplay->setText(code);
    clearHightlights();
}

void PseudoIO::setErrorLines(const QList<int>& positions) const {
    QTextCursor cursor(codeDisplay->document());
    QList<QTextEdit::ExtraSelection> highlights;
    for (int position : positions) {
        QTextEdit::ExtraSelection highlight;
        highlight.cursor = cursor;
        highlight.cursor.setPosition(position);
        highlight.cursor.movePosition(QTextCursor::StartOfLine);
        highlight.cursor.movePosition(QTextCursor::EndOfLine);
        highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
        highlight.format.setBackground(ERROR_HIGHLIGHT);
        highlights.append(highlight);
    }
    codeDisplay->setExtraSelections(highlights);
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

void PseudoIO::clearInput() const {
    commandInput->clear();
}

void PseudoIO::clearHightlights() const {
    QList<QTextEdit::ExtraSelection> highlights;
    codeDisplay->setExtraSelections(highlights);
}
