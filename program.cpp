#include "program.h"

Program::Program(PseudoIO* io) {
    context = new Runtime(io);
    connect(context->io, &PseudoIO::receivedInput, this, QOverload<QString, const Value*>::of(&Program::input));
}

const QPair<int, QString> Program::parseLine(QString line) const {
    QString lineStr = line.section(' ', 0, 0);
    int lineNo;
    bool ok;
    lineNo = lineStr.toInt(&ok);
    if (!ok) {
        SyntaxError* error = new SyntaxError("Wrong line number format \"" + lineStr + "\"");
        error->setContext(line);
        throw error;
    }
    if (lineNo <= 0 || lineNo > 1000000) {
        SyntaxError* error = new SyntaxError("Line number should be a positive integer below 1000000");
        error->setContext(line);
        throw error;
    }
    QString content = line.trimmed().section(' ', 1, -1);
    return QPair<int, QString>(lineNo, content);
}

void Program::edit(QString command) {
    if (command.isEmpty()) { // Ignore empty line
        return;
    }
    QPair<int, QString> result = parseLine(command);
    int line = result.first;
    QString content = result.second;

    if (content.isEmpty()) {
        code.remove(line);
    } else {
        code.insert(line, content);
    }
    printCode();
}

void Program::load(QString filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        edit(in.readLine());
    }
    file.close();
    context->io->clear();
    printCode();
}

// Execute single statement
void Program::stepExecute() {
    QMap<int, QString>::const_iterator currentLine = context->pc;
    int lineNo = currentLine.key();
    QString content = currentLine.value();
    qDebug() << "executing line " << lineNo;
    try {
        const Statement* instruction = Statement::parse(content);
        if (instruction->error) {
            throw instruction->error;
        }
        instruction->execute(context);
    } catch (Exception* error) {
        context->status = HALT;
        exitDebug(false);
        error->setContext(QString("%1 %2").arg(lineNo).arg(content));
        context->io->addErrorLine(linePosition(currentLine));
        throw;
    }
    ++context->pc;
    if (context->pc == code.constEnd() && context->status != GOTO) {
        context->status = HALT;
    }
    // Handle goto statements, find corresponding const_iterator
    if (context->status == GOTO) {
        QMap<int, QString>::const_iterator destination = code.constFind(context->gotoDst);
        if (destination == code.constEnd()) {
            context->io->addErrorLine(linePosition(context->pc - 1));
            exitDebug(false);
            throw new RuntimeError(QString("Jump destination \"%1\" doesn't exist").arg(context->gotoDst));
        }
        context->pc = destination;
        context->status = OK;
        context->gotoDst = 0;
    }
}

void Program::run() {
    while (context->status != INTERRUPT && context->status != HALT) {
        if (debug) {
            step();
        } else {
            stepExecute();
        }
    }
    if (context->status == HALT && debug) {
        exitDebug();
    }
}

void Program::init() {
    if (code.empty()) {
        throw new Exception("No program to run");
    }
    context->init();
    context->pc = code.constBegin();
}

void Program::start() {
    if (!debug) {
        context->io->clearOutput();
        context->io->clearState();
        init();
        printAst();
    }
    run();
}

void Program::step() {
    if (!debug) {
        printAst();
        context->io->clearOutput();
        context->io->clearState();
        init();
        enterDebug();
    } else {
        if (context->status == INTERRUPT) { // Awaiting input
            return;
        }
        if (context->status == HALT) { // Exited normally
            exitDebug();
            return;
        }
        stepExecute();
    }
    highlightCurrentLine();
    printCurrentAst();
}

void Program::input(QString identifier, const Value* value) {
    context->symbols->setValue(identifier, value);
    if (context->status != INTERRUPT) { // Program not awaiting input
        return;
    }
    context->status = OK;
    try {
        if (debug) {
            step();
        } else {
            run();
        }
    } catch (Exception* error) {
        showErrorMessage(*error);
    }
}

void Program::clear() {
    code.clear();
    context->reset();
}

void Program::printCode() const {
    QString content;
    QMap<int, QString>::const_iterator i;
    for (i = code.constBegin(); i != code.constEnd(); ++i) {
        content += QString("%1 %2\n").arg(i.key()).arg(i.value());
    }
    context->io->setCode(content);
}

void Program::printAst() const {
    QMap<int, QString>::const_iterator i;
    QString ast, line;
    int position = 1, lineNo;
    QList<int> errorPositions;
    Statement const* instruction;
    for (i = code.constBegin(); i != code.constEnd(); ++i) {
        lineNo = i.key();
        line = i.value();
        instruction = Statement::parse(line);
        ast += lineAst(lineNo, instruction);
        if (instruction->error) {
            errorPositions.append(position);
        }
        position += lineLength(lineNo, line);
    }
    context->io->setAst(ast);
    context->io->setErrorLines(errorPositions);
}

void Program::printCurrentAst() const {
    QMap<int, QString>::const_iterator currentLine = context->pc;
    if (currentLine == code.end()) {
        return;
    }
    Statement const* instruction = Statement::parse(currentLine.value());
    context->io->setAst(lineAst(currentLine.key(), instruction));
}

Runtime* Program::getContext() {
    return context;
}

void Program::highlightCurrentLine() const {
    QMap<int, QString>::const_iterator currentLine = context->pc;
    if (currentLine == code.end()) {
        return;
    }
    context->io->setCurrentLine(linePosition(currentLine));
}

void Program::enterDebug() {
    debug = true;
    emit enteredDebug();
}

void Program::exitDebug(bool exitedNormally) {
    debug = false;
    if (exitedNormally) {
        QMessageBox::information(this, "Debug", "Program exited normally.");
    }
    emit exitedDebug();
}

inline int Program::lineLength(int lineNo, QString line) const {
    // length of line number + statement + 1 whitespace + 1 '\0'
    return QString::number(lineNo).length() + line.length() + 2;
}

int Program::linePosition(const QMap<int, QString>::const_iterator& line) const {
    int position = 1, lineNo;
    QString content;
    for (QMap<int, QString>::const_iterator i = code.constBegin(); i != line; ++i) {
        lineNo = i.key();
        content = i.value();
        position += lineLength(lineNo, content);
    }
    return position;
}

inline QString Program::lineAst(int lineNo, const Statement* instruction) const {
    return QString("%1 %2\n").arg(lineNo).arg(instruction->ast());
}

Program::~Program() {
    delete context;
}
