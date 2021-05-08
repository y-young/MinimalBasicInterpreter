#ifndef PROGRAM_H
#define PROGRAM_H

#include <QDebug>
#include <QFile>
#include <QMap>
#include <QMessageBox>
#include <QPair>
#include <QString>
#include <QWidget>

#include "exceptions.h"
#include "runtime.h"
#include "statement.h"
#include "value.h"

class Program: public QWidget {
    Q_OBJECT
  private:
    QMap<int, QString> code;
    Runtime* context = nullptr;
    bool debug = false;
    void init();
    const QPair<int, QString> parseLine(QString line) const;
    void enterDebug();
    void exitDebug(bool exitedNormally = true);
    void stepExecute();
    void input(QString identifier, const Value* value);
    void highlightCurrentLine() const;
    void printCurrentAst() const;
    inline int lineLength(int lineNo, QString line) const;
    int linePosition(const QMap<int, QString>::const_iterator& line) const;
    inline QString lineAst(int lineNo, const Statement* instruction) const;

  public:
    Program(PseudoIO* io);
    void edit(QString command);
    void load(QString filename);
    void run();
    void start();
    void step();
    void clear();
    void printCode() const;
    void printAst() const;
    Runtime* getContext();
    ~Program();

  signals:
    void enteredDebug();
    void exitedDebug();
};

#endif // PROGRAM_H
