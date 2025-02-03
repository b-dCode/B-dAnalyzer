//
// Created by artem on 02.02.25.
//

#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include "Token.h"
#include "token_types.h"

typedef PreprocessingTokenType PPTokenType;
typedef Token<TokenType> token;
typedef Token<PPTokenType> pp_token;
typedef std::string::iterator StringIterator;

class Scanner {
public:
    //    static std::vector<token> Parse(std::string_view line, size_t line_index);
    void Preproccess();

    [[nodiscard]] std::string GetText() const;

    void SetText(const std::string &text);

    [[nodiscard]] std::vector<token> GetTokens() const;

    [[nodiscard]] std::vector<pp_token> GetPpTokens() const;

private:
    StringIterator GetPPNumber(StringIterator token_start);

    StringIterator GetIdentifier(StringIterator token_start);

    // token_start указывает на "
    StringIterator GetStringLiteral(StringIterator token_start);

    // token_start указывает на '
    StringIterator GetCharacterLiteral(StringIterator token_start);

    // token_start указывает на < или "
    StringIterator GetHeaderName(StringIterator token_start);

    StringIterator GetPunctuator(StringIterator token_start);

    static bool isAlnum_(int c);

    std::vector<token> tokens_;
    std::vector<pp_token> pp_tokens_;
    std::string text_;

    static bool isIdentifierContinue(int c);

    static bool isHexDigit(int c);

    static bool isOctDigit(int c);

    static bool isIdentifierStart(int c);

    static bool isSimpleEscapeSequenceChar(int c);

    /*
     * Возвращает итератор на первый символ, который не получится принять за префикс
     * " или ' для строки или символа соответственно и другое значение для другого токена
     * Префиксы: u U L u8
     */
    StringIterator getEncodingPrefix(StringIterator start);
};

#endif //SCANNER_H
