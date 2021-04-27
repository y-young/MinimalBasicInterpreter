#ifndef PSEUDOIO_H
#define PSEUDOIO_H

#include <QObject>
#include <QString>
#include <QTextBrowser>

class PseudoIO: public QObject {
    Q_OBJECT
  private:
    QTextBrowser* codeDisplay = nullptr;
    QTextBrowser* outputDisplay = nullptr;
    QTextBrowser* astDisplay = nullptr;

  public:
    PseudoIO(QTextBrowser* code, QTextBrowser* output, QTextBrowser* ast);
    void input(QString identifier);
    void input(QString identifier, QString stream);
    void output(int value) const;
    void output(QString content) const;
    void setCode(QString code) const;
    void setAst(QString ast) const;
    void clear() const;
    void clearOutput() const;
    void clearAst() const;

  signals:
    void requestInput(QString identifier);
    void receivedInput(QString identifier, int input);
};

#endif // PSEUDOIO_H
