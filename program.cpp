#include "program.h"

Program::Program() {
}

const QPair<int, QString> Program::parseLine(const QString& line) const {
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

void Program::edit(const QString& command) {
    QPair<int, QString> result = parseLine(command);
    int line = result.first;
    QString content = result.second;

    if (content.isEmpty()) {
        data.remove(line);
    } else {
        data.insert(line, content);
    }
}

void Program::load(const QString& filename) {
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

void Program::clear() {
    data.clear();
}

const QString Program::text() const {
    QString content;
    QMap<int, QString>::const_iterator i;
    for (i = data.constBegin(); i != data.constEnd(); ++i) {
        content += QString("%1 %2\n").arg(i.key()).arg(i.value());
    }
    return content;
}
