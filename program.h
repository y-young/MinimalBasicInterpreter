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
    Runtime context;
    const QPair<int, QString> parseLine(QString line) const;
    void stepExecute();
    void input(QString identifier, int value);

  public:
    Program();
    void edit(QString command);
    void load(QString filename);
    void run();
    void start();
    QString printAst() const;
    void clear();
    const QString text() const;
    PseudoIO& io();
};

#endif // PROGRAM_H
