#ifndef UTILS_H
#define UTILS_H

#include <QMessageBox>
#include <QRegExp>
#include <QString>

#include "exceptions.h"

// Indent each line in a string with 4 spaces
QString indent(QString content);

void showErrorMessage(const Exception& error);

#endif // UTILS_H
