//
// Created by artem on 02.02.25.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <utility>
#include "token_types.h"

template<typename T>
class Token {
public:
    Token() = default;

    explicit Token(const T type, const size_t line, const size_t column) : type_(type), line_(line),
        column_(column) {
    }

    Token(const T type, std::string name, const size_t line,
          const size_t column) : type_(type), name_(std::move(name)), line_(line), column_(column) {
    }

    [[nodiscard]] std::string GetName() const {
        return name_;
    }

    [[nodiscard]] TokenType GetType() const {
        return type_;
    }

    void SetType(TokenType type) {
        this->type_ = type;
    }

    void SetName(const std::string &name) {
        this->name_ = name;
    }

private:
    T type_;
    std::string name_;
    size_t line_ = 0;
    size_t column_ = 0;
};


#endif //TOKEN_H
