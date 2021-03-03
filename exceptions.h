#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>
#include <exception>
#include <string>

class Exception: public std::exception {
  protected:
    QString info;
    const QString type = "Exception";

  public:
    Exception(QString msg) {
        info = msg;
    }

    Exception(QString msg, QString context) {
        info = QString("%1\n\t> %2").arg(msg).arg(context);
    }

    virtual const char* what() const noexcept override {
        std::string* msg = new std::string();
        const QString message = QString("%1: %2").arg(type).arg(info);
        *msg = message.toStdString();
        return msg->c_str();
    }
};

class SyntaxError: public Exception {
  protected:
    const QString type = "SyntaxError";

  public:
    SyntaxError(QString msg): Exception(msg) {
    }
    SyntaxError(QString msg, QString context): Exception(msg, context) {
    }
};

class RuntimeError: public Exception {
  protected:
    const QString type = "RuntimeError";

  public:
    RuntimeError(QString msg): Exception(msg) {
    }
    RuntimeError(QString msg, QString context): Exception(msg, context) {
    }
};

#endif // EXCEPTIONS_H
