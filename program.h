#ifndef PROGRAM_H
#define PROGRAM_H

#include <QDebug>
#include <QFile>
#include <QMap>
#include <QPair>
#include <QString>

#include "exceptions.h"

class Program {
  private:
    QMap<int, QString> data;
    const QPair<int, QString> parseLine(const QString& line) const;

  public:
    Program();
    void edit(const QString& command);
    void load(const QString& filename);
    void clear();
    const QString text() const;
};

#endif // PROGRAM_H
