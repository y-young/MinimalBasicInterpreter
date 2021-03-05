#include "program.h"

Program::Program() {
    connect(&context.io, &PseudoIO::receivedInput, this, QOverload<QString, int>::of(&Program::input));
}

const QPair<int, QString> Program::parseLine(QString line) const {
    QString lineStr = line.section(' ', 0, 0);
    int lineNo;
    bool ok;
    lineNo = lineStr.toInt(&ok);
    if (!ok) {
        throw SyntaxError("Wrong line number format \"" + lineStr + "\"", line);
    }
    QString content = line.section(' ', 1, -1);
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
}

void Program::load(QString filename) {
    clear();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        edit(in.readLine());
    }
    file.close();
}

void Program::stepExecute() {
    // execute single statement
    int id = context.pc.key();
    qDebug() << id;
    try {
        const Statement* instruction = Statement::parse(context.pc.value());
        instruction->execute(context);
    } catch (const Exception& error) {
        QMessageBox::critical(this, "Error", error.what());
    }
    ++context.pc;
    if (context.pc == data.constEnd()) {
        context.status = INTERRUPT;
    }
}

void Program::run() {
    while (context.status != INTERRUPT && context.status != HALT) {
        // handle goto statements, find corresponding const_iterator
        if (context.status == GOTO) {
            context.pc = data.find(context.gotoDst);
            context.status = OK;
            context.gotoDst = 0;
        }
        stepExecute();
    }
}

void Program::start() {
    if (data.empty()) {
        throw Exception("No program to run");
    }
    context.reset();
    context.pc = data.constBegin();
    run();
}

void Program::input(QString identifier, int value) {
    context.symbols.setValue(identifier, value);
    context.status = OK;
    run();
}

QString Program::printAst() const {
    QMap<int, QString>::const_iterator i;
    QString ast;
    Statement const* instruction;
    for (i = data.constBegin(); i != data.constEnd(); ++i) {
        instruction = Statement::parse(i.value());
        ast += QString("%1 %2\n").arg(i.key()).arg(instruction->ast());
    }
    return ast;
}

void Program::clear() {
    data.clear();
    context.reset();
}

const QString Program::text() const {
    QString content;
    QMap<int, QString>::const_iterator i;
    for (i = data.constBegin(); i != data.constEnd(); ++i) {
        content += QString("%1 %2\n").arg(i.key()).arg(i.value());
    }
    return content;
}

PseudoIO& Program::io() {
    return context.io;
}
