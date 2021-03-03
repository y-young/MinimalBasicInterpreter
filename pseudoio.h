#ifndef PSEUDOIO_H
#define PSEUDOIO_H

#include <QObject>
#include <QString>

class PseudoIO: public QObject {
    Q_OBJECT
  public:
    PseudoIO();
    void input(QString identifier);
    void input(QString identifier, QString stream);
    void output(int value) const;
    void output(QString content) const;

  signals:
    void requestInput(QString identifier);
    void printOutput(QString content) const;
    void receivedInput(QString identifier, int input);
};

#endif // PSEUDOIO_H
