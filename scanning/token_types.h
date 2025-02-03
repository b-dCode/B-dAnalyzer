//
// Created by artem on 02.02.25.
//

#ifndef TOKENS_H
#define TOKENS_H

#include <array>

enum class PreprocessingTokenType {
    header_name,
    identifier,
    pp_number,
    character_literal,
    string_literal,
    punctuator,
    universal_character_name,
    non_white_space_character
};

enum class TokenType {
    Undefined,
    Keyword,
    Identifier,
    Constant,
    String_literal,
    Punctuator
};

constexpr std::array<std::string_view, 54> keywords = {
    "alignas", "alignof", "auto", "bool", "break", "case", "char", "const",
    "constexpr", "continue", "default", "do", "double", "else",
    "enum", "extern", "false", "float", "for", "goto", "if", "inline", "int",
    "long", "nullptr", "register", "restrict", "return", "short", "signed",
    "sizeof", "static", "static_assert", "struct", "switch", "thread_local",
    "true", "typedef", "typeof", "typeof_unqual", "union", "unsigned", "void",
    "volatile", "while", "_Atomic", "_BitInt", "_Complex", "_Decimal128",
    "_Decimal32", "_Decimal64", "_Generic", "_Lengthof", "_Noreturn"
};
constexpr std::array<std::string_view, 55> punctuators = {
    "[", "]", "(", ")", "{", "}", ".", "->",
    "++", "--", "&", "*", "+", "-", "~", "!",
    "/", "%",
    "<<", ">>",
    "<", ">", "<=", ">=", "==", "!=",
    "^", "|", "&&", "||",
    "?", ":",
    "::", ";", "...",
    "=", "*=", "/=", "%=", "+=", "-=", "<<=", ">>=", "&=", "^=", "|=",
    ",", "#", "##",
    "<:", ":>", "<%", "%>", "%:", "%:%:"
};
#endif //TOKENS_H
