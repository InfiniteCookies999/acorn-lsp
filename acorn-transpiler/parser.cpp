#include "parser.h"

#include <iostream>
#include <ctype.h>

namespace parser {

TSTokenizer::TSTokenizer(std::string filename)
    : file() {
    file.open(filename);
}

#define simple_set(TTkind)    \
do {                          \
    token.kind = TTkind;      \
    token.content = cur_char; \
    return token;             \
} while(0)

void TSTokenizer::skip_comment(char &cur_char) {
    if (file.peek() == '/') {
        while (cur_char != '\n') {
            file.get(cur_char);
        }
    }

    if (file.peek() == '*' ) {
        for (;;) {
            if (cur_char == '*' && file.peek() == '/') {
                file.get(cur_char);
                file.get(cur_char);
                break;
            }
            file.get(cur_char);
        }
    }
    while (isspace(cur_char)) {
        file.get(cur_char);
    }
}

Token TSTokenizer::get_next_token() {
    using enum TokenType;

    if (!file.is_open()) {
        std::cerr << "file not open" << std::endl;
        return Token {
            .kind = EOFToken,
            .content = " "
        };
    }

    if (file.eof()) {
        return Token {
            .kind = EOFToken,
            .content = " "
        };
    }

    Token token;
    char cur_char;
    file.get(cur_char);

    while (isspace(cur_char)) file.get(cur_char);

    if (cur_char == '/' && (file.peek() == '/' || file.peek() == '*')) {
        skip_comment(cur_char);
    }

    switch (cur_char) {
        case ':': simple_set(Colon);
        case '{': simple_set(LBrace);
        case '}': simple_set(RBrace);
        case '|': simple_set(Pipe);
        case '?': simple_set(QuestionMark);
        case ';': simple_set(Semicolon);
        default: break;
    }

    token.content = cur_char;
    while (isalnum(file.peek())) {
        file.get(cur_char);
        token.content += cur_char;
    }

    if (token.content == "extends") {
        token.kind = Keyword;
        return token;
    }

    if (token.content == "interface") {
        token.kind = Keyword;
        return token;
    }
    token.kind = Identifier;
    return token;
}

#undef simple_set

}
