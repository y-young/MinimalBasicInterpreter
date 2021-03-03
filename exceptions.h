#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>
#include <exception>
#include <string>

class Exception: public std::exception {
  protected:
    QString info;
    QString message;
    const QString type = "Exception";

  public:
    Exception(const QString& msg, const QString& command) {
        info = QString("%1\n\t> %2").arg(msg).arg(command);
        message = QString("%1: %2").arg(type).arg(info);
    }
    virtual const char* what() const noexcept override {
        std::string* msg = new std::string();
        *msg = message.toStdString();
        return msg->c_str();
    }
};

class SyntaxError: public Exception {
  protected:
    const QString type = "SyntaxError";

  public:
    SyntaxError(const QString& msg, const QString& command): Exception(msg, command) {
    }
};

class RuntimeError: public Exception {
  protected:
    const QString type = "RuntimeError";

  public:
    RuntimeError(const QString& msg, const QString& command): Exception(msg, command) {
    }
};

#endif // EXCEPTIONS_H
