#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <QString>
#include <exception>
#include <string>

class Exception: public std::exception {
  protected:
    QString info;
    QString context;
    const char* message(const QString type) const {
        std::string* msg = new std::string();
        QString errorMsg = QString("%1: %2").arg(type).arg(info);
        if (!context.isEmpty()) {
            errorMsg += "\n\t> " + context;
        }
        *msg = errorMsg.toStdString();
        return msg->c_str();
    }

  public:
    Exception(QString msg) {
        info = msg;
    }

    void setContext(const QString ctx) {
        context = ctx;
    }

    virtual const char* what() const noexcept override {
        return message("Exception");
    }
};

class SyntaxError: public Exception {
  public:
    SyntaxError(QString msg): Exception(msg) {
    }
    const char* what() const noexcept override {
        return message("SyntaxError");
    }
};

class RuntimeError: public Exception {
  public:
    RuntimeError(QString msg): Exception(msg) {
    }
    const char* what() const noexcept override {
        return message("RuntimeError");
    }
};

#endif // EXCEPTIONS_H
