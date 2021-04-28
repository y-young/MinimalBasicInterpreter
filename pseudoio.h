#ifndef PSEUDOIO_H
#define PSEUDOIO_H

#include <QColor>
#include <QLineEdit>
#include <QObject>
#include <QString>
#include <QTextBrowser>

#include "value.h"

const QColor CURRENT_HIGHLIGHT(100, 255, 100), ERROR_HIGHLIGHT(255, 100, 100);

class PseudoIO: public QObject {
    Q_OBJECT
  private:
    QTextBrowser* codeDisplay = nullptr;
    QTextBrowser* outputDisplay = nullptr;
    QTextBrowser* astDisplay = nullptr;
    QTextBrowser* stateDisplay = nullptr;
    QLineEdit* commandInput = nullptr;
    bool awaitingInput = false;
    QString awaitingIdentifier;
    QList<QTextEdit::ExtraSelection> errorHighlights;
    void handleInput();

  public:
    PseudoIO(QTextBrowser* code, QTextBrowser* output, QTextBrowser* ast, QTextBrowser* state, QLineEdit* command);
    void requestInput(QString identifier);
    void finishInput(QString stream);
    void output(const Value* value) const;
    void setCode(QString code);
    void setCurrentLine(int position) const;
    void setErrorLines(const QList<int>& positions);
    void addErrorLine(int position);
    void setAst(QString ast) const;
    void setState(QString state) const;
    void clear() const;
    void clearOutput() const;
    void clearAst() const;
    void clearInput() const;
    void clearState() const;
    void clearHightlights();

  signals:
    void receivedInput(QString identifier, const Value* input);
    void receivedCommand(QString command);
};

#endif // PSEUDOIO_H
