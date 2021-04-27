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

class Program: public QWidget {
    Q_OBJECT
  private:
    QMap<int, QString> data;
    Runtime* context = nullptr;
    const QPair<int, QString> parseLine(QString line) const;
    void stepExecute();
    void input(QString identifier, int value);

  public:
    Program(PseudoIO* io);
    void edit(QString command);
    void load(QString filename);
    void run();
    void start();
    void clear();
    void printCode() const;
    void printAst() const;
    void printState() const;
    Runtime* getContext();
    ~Program();
};

#endif // PROGRAM_H
