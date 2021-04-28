#include "pseudoio.h"

#include <QDebug>

PseudoIO::PseudoIO(QTextBrowser* code, QTextBrowser* output, QTextBrowser* ast, QTextBrowser* state, QLineEdit* command)
    : codeDisplay(code), outputDisplay(output), astDisplay(ast), stateDisplay(state), commandInput(command) {
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

void PseudoIO::setCode(QString code) {
    codeDisplay->setText(code);
    clearHightlights();
}

void PseudoIO::setState(QString state) const {
    stateDisplay->setText(state);
}

void PseudoIO::setCurrentLine(int position) const {
    QTextCursor cursor(codeDisplay->document());
    QTextEdit::ExtraSelection highlight;
    highlight.cursor = cursor;
    try {
        highlight.cursor.setPosition(position);
    } catch (const QString) {
        return;
    }
    highlight.cursor.movePosition(QTextCursor::StartOfLine);
    highlight.cursor.movePosition(QTextCursor::EndOfLine);
    highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
    highlight.format.setBackground(CURRENT_HIGHLIGHT);
    QList<QTextEdit::ExtraSelection> currentHighlight;
    currentHighlight.append(highlight);
    codeDisplay->setExtraSelections(errorHighlights + currentHighlight);
}

void PseudoIO::setErrorLines(const QList<int>& positions) {
    QTextCursor cursor(codeDisplay->document());
    for (int position : positions) {
        QTextEdit::ExtraSelection highlight;
        highlight.cursor = cursor;
        highlight.cursor.setPosition(position);
        highlight.cursor.movePosition(QTextCursor::StartOfLine);
        highlight.cursor.movePosition(QTextCursor::EndOfLine);
        highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
        highlight.format.setBackground(ERROR_HIGHLIGHT);
        errorHighlights.append(highlight);
    }
    codeDisplay->setExtraSelections(errorHighlights);
}

void PseudoIO::addErrorLine(int position) {
    QList<int> positions;
    positions.append(position);
    setErrorLines(positions);
}

void PseudoIO::setAst(QString ast) const {
    astDisplay->setText(ast);
}

void PseudoIO::clear() const {
    codeDisplay->clear();
    outputDisplay->clear();
    astDisplay->clear();
    stateDisplay->clear();
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

void PseudoIO::clearState() const {
    stateDisplay->clear();
}

void PseudoIO::clearHightlights() {
    errorHighlights.clear();
    QList<QTextEdit::ExtraSelection> highlights;
    codeDisplay->setExtraSelections(highlights);
}
