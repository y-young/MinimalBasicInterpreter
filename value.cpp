#include "value.h"

// Value
Value::Value() {
}

Value::~Value() {
}

// IntegerValue
IntegerValue::IntegerValue(int v): value(v) {
}

QString IntegerValue::type() const {
    return "INT";
}

QString IntegerValue::toString() const {
    return QString::number(value);
}

const Value* IntegerValue::operator+(const Value& other) const {
    return new IntegerValue(value + reinterpret_cast<const IntegerValue&>(other).value);
}

const Value* IntegerValue::operator-(const Value& other) const {
    return new IntegerValue(value - reinterpret_cast<const IntegerValue&>(other).value);
}

const Value* IntegerValue::operator*(const Value& other) const {
    return new IntegerValue(value * reinterpret_cast<const IntegerValue&>(other).value);
}

const Value* IntegerValue::operator/(const Value& other) const {
    int divisor = reinterpret_cast<const IntegerValue&>(other).value;
    if (divisor == 0) {
        throw new RuntimeError("Division by 0");
    }
    return new IntegerValue(value / divisor);
}

const Value* IntegerValue::pow(const Value& other) const {
    return new IntegerValue(qPow(value, reinterpret_cast<const IntegerValue&>(other).value));
}

bool IntegerValue::operator<(const Value& other) const {
    return value < reinterpret_cast<const IntegerValue&>(other).value;
}

bool IntegerValue::operator>(const Value& other) const {
    return value > reinterpret_cast<const IntegerValue&>(other).value;
}

bool IntegerValue::operator==(const Value& other) const {
    return value == reinterpret_cast<const IntegerValue&>(other).value;
}

QString IntegerValue::ast() const {
    return toString();
}

IntegerValue::~IntegerValue() {
}

// StringValue
StringValue::StringValue(QString v): value(v) {
}

QString StringValue::type() const {
    return "STR";
}

QString StringValue::toString() const {
    return value;
}

const Value* StringValue::operator+(const Value&) const {
    throw new RuntimeError("Unsupported operation '+' for strings.");
}

const Value* StringValue::operator-(const Value&) const {
    throw new RuntimeError("Unsupported operation '-' for strings.");
}

const Value* StringValue::operator*(const Value&) const {
    throw new RuntimeError("Unsupported operation '*' for strings.");
}

const Value* StringValue::operator/(const Value&) const {
    throw new RuntimeError("Unsupported operation '/' for strings.");
}

const Value* StringValue::pow(const Value&) const {
    throw new RuntimeError("Unsupported operation '**' for strings.");
}

bool StringValue::operator<(const Value&) const {
    throw new RuntimeError("Unsupported operation '<' for strings.");
}

bool StringValue::operator>(const Value&) const {
    throw new RuntimeError("Unsupported operation '>' for strings.");
}

bool StringValue::operator==(const Value&) const {
    throw new RuntimeError("Unsupported operation '=' for strings.");
}

QString StringValue::ast() const {
    return '"' + toString() + '"';
}

StringValue::~StringValue() {
}
