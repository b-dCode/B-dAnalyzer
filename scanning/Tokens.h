//
// Created by artem on 02.02.25.
//

#ifndef TOKENS_H
#define TOKENS_H


enum Preprocessing_token {
    header_name,
    identifier,
    pp_number,
    character_literal,
    string_literal,
    punctuator,
    universal_character_name,
    non_white_space_character
};

enum Token {
    Keyword,
    Identifier,
    Constant,
    String_literal,
    Punctuator
};

#endif //TOKENS_H
