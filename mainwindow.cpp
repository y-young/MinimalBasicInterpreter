#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTextStream>

#include "exceptions.h"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->CommandInput->setFocus();
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
    program.load(filename);
    ui->CodeDisplay->setText(program.text());
}

void MainWindow::clear() {
    program.clear();
    ui->CodeDisplay->clear();
    ui->OutputDisplay->clear();
    ui->ASTDisplay->clear();
}

void MainWindow::showHelp() {
    const QString helpText = "\
            RUN: Run the program\n\
            LOAD: Load program from a source code file\n\
            CLEAR: Clear current program\n\
            HELP: Show this help text\n";
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Help");
    msgBox.setText("Commands:");
    msgBox.setInformativeText(helpText);
    msgBox.exec();
}

void MainWindow::executeCommand() {
    QString command = ui->CommandInput->text();
    if (command.isEmpty()) {
        return;
    }

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
        try {
            program.edit(command);
        } catch (const Exception& error) {
            QMessageBox::critical(this, "Error", error.what());
        }
        ui->CodeDisplay->setText(program.text());
    }
    ui->CommandInput->clear();
}

MainWindow::~MainWindow() {
    delete ui;
}
