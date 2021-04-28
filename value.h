#ifndef VALUE_H
#define VALUE_H

#include <QString>
#include <QtMath>

#include "exceptions.h"

class Value {
  public:
    Value();
    virtual QString type() const = 0;
    virtual QString toString() const = 0;
    virtual ~Value();
    virtual const Value* operator+(const Value& other) const = 0;
    virtual const Value* operator-(const Value& other) const = 0;
    virtual const Value* operator*(const Value& other) const = 0;
    virtual const Value* operator/(const Value& other) const = 0;
    virtual const Value* pow(const Value& other) const = 0;
    virtual bool operator<(const Value& other) const = 0;
    virtual bool operator>(const Value& other) const = 0;
    virtual bool operator==(const Value& other) const = 0;
    virtual QString ast() const = 0;
};

class IntegerValue: public Value {
  private:
    int value;

  public:
    IntegerValue(int v);
    QString type() const override;
    QString toString() const override;
    const Value* operator+(const Value& other) const override;
    const Value* operator-(const Value& other) const override;
    const Value* operator*(const Value& other) const override;
    const Value* operator/(const Value& other) const override;
    const Value* pow(const Value& other) const override;
    bool operator<(const Value& other) const override;
    bool operator>(const Value& other) const override;
    bool operator==(const Value& other) const override;
    QString ast() const override;
    ~IntegerValue();
};

class StringValue: public Value {
  private:
    QString value;

  public:
    StringValue(QString v);
    QString type() const override;
    QString toString() const override;
    const Value* operator+(const Value& other) const override;
    const Value* operator-(const Value& other) const override;
    const Value* operator*(const Value& other) const override;
    const Value* operator/(const Value& other) const override;
    const Value* pow(const Value& other) const override;
    bool operator<(const Value& other) const override;
    bool operator>(const Value& other) const override;
    bool operator==(const Value& other) const override;
    QString ast() const override;
    ~StringValue();
};

#endif // VALUE_H
