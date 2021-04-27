#include "program.h"

Program::Program(PseudoIO* io) {
    context = new Runtime(io);
    connect(context->io, &PseudoIO::receivedInput, this, QOverload<QString, int>::of(&Program::input));
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
    QString content = line.simplified().section(' ', 1, -1);
    return QPair<int, QString>(lineNo, content);
}

void Program::edit(QString command) {
    QPair<int, QString> result = parseLine(command);
    int line = result.first;
    QString content = result.second;

    if (content.isEmpty()) {
        data.remove(line);
    } else {
        data.insert(line, content);
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
    printCode();
}

void Program::stepExecute() {
    // execute single statement
    int lineNo = context->pc.key();
    QString line = context->pc.value();
    qDebug() << "executing line " << lineNo;
    try {
        const Statement* instruction = Statement::parse(line);
        if (instruction->error) {
            throw instruction->error;
        }
        instruction->execute(context);
    } catch (Exception* error) {
        context->status = HALT;
        error->setContext(QString("%1 %2").arg(lineNo).arg(line));
        throw;
        return;
    }
    ++context->pc;
    if (context->pc == data.constEnd() && context->status != GOTO) {
        context->status = HALT;
    }
}

void Program::run() {
    while (context->status != INTERRUPT && context->status != HALT) {
        // handle goto statements, find corresponding const_iterator
        if (context->status == GOTO) {
            context->pc = data.constFind(context->gotoDst);
            if (context->pc == data.constEnd()) {
                throw new RuntimeError(QString("Jump destination \"%1\" doesn't exist").arg(context->gotoDst));
            }
            context->status = OK;
            context->gotoDst = 0;
        }
        stepExecute();
    }
}

void Program::start() {
    if (data.empty()) {
        throw new Exception("No program to run");
    }
    context->pc = data.constBegin();
    context->status = OK;
    run();
}

void Program::input(QString identifier, int value) {
    context->symbols.setValue(identifier, value);
    if (context->status != INTERRUPT) { // Console input
        return;
    }
    context->status = OK;
    try {
        run();
    } catch (Exception* error) {
        showErrorMessage(*error);
    }
}

void Program::printAst() const {
    QMap<int, QString>::const_iterator i;
    QString ast, line;
    int position = 0, lineNo;
    QList<int> errorPositions;
    Statement const* instruction;
    for (i = data.constBegin(); i != data.constEnd(); ++i) {
        lineNo = i.key();
        line = i.value();
        instruction = Statement::parse(line);
        ast += QString("%1 %2\n").arg(lineNo).arg(instruction->ast());
        if (instruction->error) {
            errorPositions.append(position + 1);
        }
        // length of line number + statement + 1 whitespace + 1 '\0'
        position += QString::number(lineNo).length() + line.length() + 2;
    }
    context->io->setAst(ast);
    context->io->setErrorLines(errorPositions);
}

void Program::clear() {
    data.clear();
    context->reset();
}

void Program::printCode() const {
    QString content;
    QMap<int, QString>::const_iterator i;
    for (i = data.constBegin(); i != data.constEnd(); ++i) {
        content += QString("%1 %2\n").arg(i.key()).arg(i.value());
    }
    context->io->setCode(content);
}

Runtime* Program::getContext() {
    return context;
}

Program::~Program() {
    delete context;
}
