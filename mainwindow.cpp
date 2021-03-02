#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTextStream>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->LoadButton, &QPushButton::released, this, QOverload<>::of(&MainWindow::load));
    connect(ui->RunButton, &QPushButton::released, this, QOverload<>::of(&MainWindow::run));
    connect(ui->ClearButton, &QPushButton::released, this, QOverload<>::of(&MainWindow::clear));
    connect(ui->CommandInput, &QLineEdit::returnPressed, this, QOverload<>::of(&MainWindow::executeCommand));
}

void MainWindow::run() {
    // Not implemented yet
}

void MainWindow::load() {
    QString filename = QFileDialog::getOpenFileName(this, "Load source code", "./", tr("BASIC Source Code (*)"));
    if (filename.isEmpty()) {
        return;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString source;
    while (!in.atEnd()) {
        source += in.readLine() + "\n";
    }
    ui->CodeDisplay->setText(source);
    file.close();
}

void MainWindow::clear() {
    ui->CodeDisplay->clear();
    ui->OutputDisplay->clear();
    ui->ASTDisplay->clear();
}

void MainWindow::showHelp() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Help");
    QString helpText = "Commands:\n\
            RUN: Run the program\n\
            LOAD: Load program from a source code file\n\
            CLEAR: Clear current program\n\
            HELP: Show this help text\n";
    msgBox.setText(helpText);
    msgBox.exec();
}

void MainWindow::executeCommand() {
    QString command = ui->CommandInput->text();
    if (command == "RUN") {
        run();
    } else if (command == "LOAD") {
        load();
    } else if (command == "CLEAR") {
        clear();
    } else if (command == "HELP") {
        showHelp();
    } else if (command == "QUIT") {
        QApplication::exit(0);
    } else {
        // executeStatement(command);
    }
    ui->CommandInput->clear();
}

MainWindow::~MainWindow() {
    delete ui;
}
