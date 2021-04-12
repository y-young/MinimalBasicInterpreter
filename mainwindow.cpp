#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    program = new Program();
    ui->CommandInput->setFocus();
    connect(ui->LoadButton, &QPushButton::released, this, QOverload<>::of(&MainWindow::load));
    connect(ui->RunButton, &QPushButton::released, this, QOverload<>::of(&MainWindow::run));
    connect(ui->ClearButton, &QPushButton::released, this, QOverload<>::of(&MainWindow::clear));
    connect(ui->CommandInput, &QLineEdit::returnPressed, this, QOverload<>::of(&MainWindow::executeCommand));

    connect(&program->io(), &PseudoIO::printOutput, this, &MainWindow::writeOutput);
    connect(&program->io(), &PseudoIO::requestInput, this, &MainWindow::awaitInput);
}

void MainWindow::run() {
    ui->OutputDisplay->clear();
    ui->ASTDisplay->clear();
    ui->ASTDisplay->setText(program->printAst());
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
    ui->CodeDisplay->setText(program->text());
}

void MainWindow::clear() {
    program->clear();
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

    ui->CommandInput->clear();

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
            stmt->execute(*program->getContext());
        } else if (command == "LIST") {
            // do nothing
        } else {
            program->edit(command);
            ui->CodeDisplay->setText(program->text());
        }
    } catch (Exception* error) {
        showErrorMessage(*error);
    }
}

void MainWindow::awaitInput(QString identifier) {
    ui->CommandInput->setText("? ");
    ui->CommandInput->setFocus();
    // Detach command input stream & Attach program input stream
    disconnect(ui->CommandInput, &QLineEdit::returnPressed, this, QOverload<>::of(&MainWindow::executeCommand));
    connect(ui->CommandInput, &QLineEdit::returnPressed, this, [=]() { this->handleInput(identifier); });
}

void MainWindow::handleInput(QString identifier) {
    QString stream = ui->CommandInput->text();
    if (!stream.startsWith("? ")) {
        ui->CommandInput->setText("? ");
        return;
    }
    ui->CommandInput->clear();
    // Detach program input stream & Reattach command input stream
    disconnect(ui->CommandInput, &QLineEdit::returnPressed, nullptr, nullptr);
    connect(ui->CommandInput, &QLineEdit::returnPressed, this, QOverload<>::of(&MainWindow::executeCommand));
    stream = stream.section(' ', 1); // Get actual input value
    program->io().input(identifier, stream);
}

void MainWindow::writeOutput(QString content) {
    ui->OutputDisplay->append(content);
}

MainWindow::~MainWindow() {
    delete ui;
    delete program;
}
