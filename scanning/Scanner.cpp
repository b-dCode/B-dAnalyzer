//
// Created by artem on 02.02.25.
//
#include <cctype>
#include <algorithm>

#include "Scanner.h"
#include "token_types.h"

std::string Scanner::GetText() const {
    return text_;
}

void Scanner::SetText(const std::string &text) {
    this->text_ = text;
}

std::vector<token> Scanner::GetTokens() const {
    return tokens_;
}

std::vector<pp_token> Scanner::GetPpTokens() const {
    return pp_tokens_;
}

void Scanner::Preproccess() {
    size_t line = 0;
    auto current_line_begin = text_.begin();
    const std::string encoding_prefix_start = "uUL";
    const std::string punctuator_start = "[({.-+&*~!/%<>=^|?:;,#";
    const std::string white_spaces = "\t\v \f\n";
    for (auto iter = text_.begin(); iter != text_.end();) {
        auto current_token_type = PPTokenType::non_white_space_character;
        auto current_token_start = iter;
        size_t column = std::distance(current_line_begin, current_token_start) + 1;
        if (*iter == '\n') {
            ++iter;
            line++;
            continue;
        }
        if (white_spaces.find(*iter) != std::string::npos) {
            ++iter;
            continue;
        }
        if ((*iter == '"' || *iter == '<') && (!pp_tokens_.empty() && pp_tokens_.back().GetName() == "define")) {
            iter = getEncodingPrefix(iter);
            current_token_type = PPTokenType::header_name;
        } else if (*iter == '\'') {
            iter = GetCharacterLiteral(iter);
            current_token_type = PPTokenType::character_literal;
        } else if (*iter == '"') {
            iter = GetStringLiteral(iter);
            current_token_type = PPTokenType::string_literal;
        } else if (encoding_prefix_start.find(*iter) != std::string::npos) {
            iter = getEncodingPrefix(iter);
            if (iter == text_.end() || (*iter != '\'' && *iter != '"')) {
                current_token_type = PPTokenType::identifier;
            } else if (*iter == '\'') {
                iter = GetCharacterLiteral(iter);
                current_token_type = PPTokenType::character_literal;
            } else {
                iter = GetStringLiteral(iter);
                current_token_type = PPTokenType::string_literal;
            }
        } else if (isIdentifierStart(*iter)) {
            iter = GetIdentifier(iter);
            current_token_type = PPTokenType::identifier;
        } else if (std::isdigit(*iter) || (*iter == '.' && iter + 1 != text_.end() && std::isdigit(*(iter + 1)))) {
            iter = GetPPNumber(text_.begin());
            current_token_type = PPTokenType::pp_number;
        } else if (punctuator_start.find(*iter) != std::string::npos) {
            iter = GetPunctuator(iter);
            current_token_type = PPTokenType::punctuator;
        } else {
            current_token_type = PPTokenType::non_white_space_character;
            ++iter;
        }
        pp_tokens_.emplace_back(current_token_type,
                                std::string(current_token_start, iter),
                                line, column);
    }
}

StringIterator Scanner::GetIdentifier(const StringIterator token_start) {
    auto iter = token_start + 1;
    for (; iter != this->text_.end() && isIdentifierContinue(*iter); ++iter) {
    }
    // ReSharper disable once CppDFALocalValueEscapesFunction
    return iter;
}

StringIterator Scanner::GetStringLiteral(const StringIterator token_start) {
    auto iter = token_start + 1;
    for (; iter != this->text_.end() && (*iter != '\n' && *iter != '"'); ++iter) {
        if (*iter == '\\' && (iter + 1) != text_.end()) ++iter;
    }
    if (iter != this->text_.end() && *iter == '"') ++iter;
    // ReSharper disable once CppDFALocalValueEscapesFunction
    return iter;
}

StringIterator Scanner::GetCharacterLiteral(const StringIterator token_start) {
    auto iter = token_start + 1;
    for (; iter != this->text_.end() && (*iter != '\n' && *iter != '\''); ++iter) {
        if (*iter == '\\' && (iter + 1) != text_.end()) ++iter;
    }
    if (iter != this->text_.end() && *iter == '\'') ++iter;
    // ReSharper disable once CppDFALocalValueEscapesFunction
    return iter;
}

StringIterator Scanner::GetHeaderName(const StringIterator token_start) {
    auto delimeter = *token_start == '<' ? '>' : '"';
    auto iter = token_start + 1;
    for (; iter != this->text_.end() && (*iter != '\n' && *iter != delimeter); ++iter) {
    }
    if (iter != this->text_.end() && *iter == delimeter) ++iter;
    // ReSharper disable once CppDFALocalValueEscapesFunction
    return iter;
}

StringIterator Scanner::GetPunctuator(StringIterator token_start) {
    auto iter = token_start + 1;
    for (bool isPunctuator = true; iter != text_.end() && isPunctuator; ++iter) {
        isPunctuator = std::ranges::find(punctuators, std::string_view(token_start, iter));
    }
    // ReSharper disable once CppDFALocalValueEscapesFunction
    return iter;
}


StringIterator Scanner::GetPPNumber(const StringIterator token_start) {
    auto iter = token_start + 1;
    for (; iter != this->text_.end(); ++iter) {
        if (isIdentifierContinue(*iter) || *iter == '.') continue;
        if ((iter + 1) != this->text_.end()) {
            if ((std::tolower(*iter) == 'e' || std::tolower(*iter) == 'p') &&
                *(iter + 1) == '+' || *(iter + 1) == '-') {
                ++iter;
                continue;
            }
            if (*iter == '\'' && isAlnum_(*(iter + 1))) continue;
        }
        break;
    }
    // ReSharper disable once CppDFALocalValueEscapesFunction
    return iter;
}

bool Scanner::isAlnum_(int c) {
    return c == '_' || std::isalnum(c);
}

//{TODO} Rewrite with Unicode
bool Scanner::isIdentifierContinue(int c) {
    return c == '_' || std::isalnum(c);
}

bool Scanner::isHexDigit(int c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'F') ||
           (c >= 'a' && c <= 'f');
}

bool Scanner::isOctDigit(int c) {
    return c >= '0' && c <= '7';
}

//{TODO} Rewrite with Unicode
bool Scanner::isIdentifierStart(int c) {
    return c == '_' || std::isalpha(c);
}

//{TODO} Rewrite with Unicode
bool Scanner::isSimpleEscapeSequenceChar(int c) {
    std::string_view pattern = "'\"?\\abfnrtv";
    return pattern.find(static_cast<char>(c)) > 0;
}

StringIterator Scanner::getEncodingPrefix(const StringIterator start) {
    if (*start == 'u' && start + 1 != text_.end() && *(start + 1) == '8') return start + 2;
    return start + 1;
}
