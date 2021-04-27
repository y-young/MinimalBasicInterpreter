#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    io = new PseudoIO(ui->CodeDisplay, ui->OutputDisplay, ui->ASTDisplay, ui->CommandInput);
    program = new Program(io);
    connect(ui->LoadButton, &QPushButton::released, this, QOverload<>::of(&MainWindow::load));
    connect(ui->RunButton, &QPushButton::released, this, QOverload<>::of(&MainWindow::run));
    connect(ui->ClearButton, &QPushButton::released, this, QOverload<>::of(&MainWindow::clear));

    connect(io, &PseudoIO::receivedCommand, this, &MainWindow::executeCommand);
}

void MainWindow::run() {
    io->clearOutput();
    program->printAst();
    try {
        program->start();
    } catch (Exception* error) {
        showErrorMessage(*error);
    }
}

void MainWindow::load() {
    QString filename = QFileDialog::getOpenFileName(this, "Load source code", "./", tr("BASIC Source Code (*)"));
    if (filename.isEmpty()) {
        return;
    }
    try {
        program->load(filename);
    } catch (Exception* error) {
        showErrorMessage(*error);
    }
}

void MainWindow::clear() {
    program->clear();
    io->clear();
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

void MainWindow::executeCommand(QString command) {
    // If this is executed by PseudoIO after emitting signal,
    // the input prompt will be cleared after program requested input.
    io->clearInput();
    try {
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
        } else if (command.startsWith("PRINT") || command.startsWith("LET") || command.startsWith("INPUT")) {
            Statement* stmt = Statement::parse(command);
            stmt->execute(program->getContext());
        } else if (command == "LIST") {
            // do nothing
        } else {
            program->edit(command);
        }
    } catch (Exception* error) {
        showErrorMessage(*error);
    }
}

MainWindow::~MainWindow() {
    delete ui;
    delete io;
    delete program;
}
