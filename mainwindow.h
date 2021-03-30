#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QFileDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTextStream>

#include "exceptions.h"
#include "program.h"
#include "statement.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow* ui;
    Program* program;
    void load();
    void run();
    void clear();
    void showHelp();
    void executeCommand();
    void awaitInput(QString identifier);
    void writeOutput(QString content);
    void handleInput(QString stream);
};
#endif // MAINWINDOW_H
