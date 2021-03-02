#include "exceptions.h"

#include <exception>
#include <string>

class SyntaxError: public std::exception {
  private:
    std::string message;

  public:
    SyntaxError(const std::string msg) {
        message = "SyntaxError: " + msg;
    }

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class RuntimeError: public std::exception {
  private:
    std::string message;

  public:
    RuntimeError(const std::string msg) {
        message = "RuntimeError: " + msg;
    }

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};
