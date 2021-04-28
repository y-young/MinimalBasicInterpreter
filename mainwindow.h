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
#include "pseudoio.h"
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
    void enterDebug();
    void exitDebug();
    ~MainWindow();

  private:
    Ui::MainWindow* ui;
    PseudoIO* io = nullptr;
    Program* program = nullptr;
    void load();
    void run();
    void debugStep();
    void clear();
    void showHelp();
    void executeCommand(QString command);
};
#endif // MAINWINDOW_H
