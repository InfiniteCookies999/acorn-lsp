#include "parser.h"

#include <ctype.h>
#include <cassert>
#include <iostream>
#include <algorithm>

namespace parser {

TSTokenizer::TSTokenizer(const std::string& filename)
    : file()
    , position(0)
    , tokens()
{
    file.open(filename);
}

bool TSTokenizer::find(std::function<bool(Token&)> func) {
    return std::find_if(tokens.begin(), tokens.end(), func) == tokens.end();
}

void TSTokenizer::skip_comment(char& cur_char) {
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

#define simple_set(TTkind)    \
do {                          \
    token.kind = TTkind;      \
    token.content = cur_char; \
    return token;             \
} while(0)

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

void TSTokenizer::clear() {
    position = 0;
    tokens.clear();
}

Token& TSTokenizer::next() {
    if (tokens.size() == position)
        tokens.push_back(get_next_token());
    return tokens[position++];
}

void TSTokenizer::skip() {
    if (tokens.size() == position)
        tokens.push_back(get_next_token());
    position++;
}

Token& TSTokenizer::peek() {
    if (tokens.size() == position)
        tokens.push_back(get_next_token());
    return tokens[position];
}

Token& TSTokenizer::operator[](std::size_t idx) {
    for (int diff = idx - tokens.size() + 1; !(tokens.size() > idx) && diff != 0; --diff)
        tokens.push_back(get_next_token());
    return tokens[idx];
}

TSAssembler::TSAssembler(const std::string& filename)
    : tokenizer(std::move(filename))
{
}

TSAssembler::~TSAssembler() {
    if (!unresolved_identifiers.empty()) {
        std::cerr << "there are unresolved identifiers:" << std::endl;
        for (const auto& ident: unresolved_identifiers) {
            std::cerr << " - " << ident << std::endl;
        }
    }
}

static constructs::type_variant lsp_types_to_type_var(const constructs::LSP::types& types)
{
    switch (types.index()) {
        case 0: return constructs::type_variant{std::get<0>(types)};
        case 1: return constructs::type_variant{std::get<1>(types)};
        case 2: return constructs::type_variant{std::get<2>(types)};
        case 3: return constructs::type_variant{std::get<3>(types)};
        case 4: return constructs::type_variant{std::get<4>(types)};
        case 5: return constructs::type_variant{std::get<5>(types)};
        case 6: return constructs::type_variant{std::get<6>(types)};
        case 7: return constructs::type_variant{std::get<7>(types)};
        case 8: return constructs::type_variant{std::get<8>(types)};
        case 9: return constructs::type_variant{std::get<9>(types)};
    }

    assert(false && "FUCK");
}

#define IF(s) if (str == s)
static constructs::LSP::types from_string(const std::string& str) {
    IF("boolean")   return constructs::LSP::boolean{};
    IF("string")    return constructs::LSP::string{};
    IF("integer")   return constructs::LSP::integer{};
    IF("uinteger")  return constructs::LSP::uinteger{};
    IF("null")      return constructs::LSP::null{};
    IF("decimal")   return constructs::LSP::decimal{};
    IF("array")     return constructs::LSP::Array{};
    IF("object")    return constructs::LSP::Object{};
    IF("LSPAny")    return constructs::LSP::Any{};

    return str;
}
#undef IF

static constructs::field_t field_parser(std::vector<constructs::identifier_t>& unresolved_identifiers, TSTokenizer& tokenizer) {
    using enum parser::TokenType;

    constructs::field_t field;
    constructs::type_t type;
    assert(tokenizer.peek().kind == Identifier && "identifier expected");

    field.ident = tokenizer.next().content;

    if (tokenizer.peek().kind == QuestionMark) {
        type.is_optional = true;
        tokenizer.skip();
    } else {
        type.is_optional = false;
    }

    assert(tokenizer.peek().kind == Colon && "colon expected");
    tokenizer.skip();

    std::vector<Token> type_tokens;
    while (tokenizer.peek().kind != Semicolon) {
        type_tokens.push_back(tokenizer.next());
    }
    tokenizer.skip();

    assert(!type_tokens.empty() && "type expected");
    if (type_tokens.size() == 1) {
        assert(type_tokens[0].kind == Identifier && "identifier expected");
        assert(!type_tokens[0].content.empty() && "content expected");

        constructs::LSP::types tv = from_string(type_tokens[0].content);
        type.type = lsp_types_to_type_var(tv);
        field.type = std::move(type);

        if (std::holds_alternative<constructs::identifier_t>(tv)) {
            if (!tokenizer.find([&](Token& token) -> bool {
                if (token.kind != Identifier) return false;
                return token.content == type_tokens[0].content;
            })) {
                unresolved_identifiers.push_back(type_tokens[0].content);
            }
        }

        return field;
    }

    auto is_pipe = [](Token& t){ return t.kind == Pipe; };

    if(std::find_if(type_tokens.begin(), type_tokens.end(), is_pipe) != type_tokens.end()) {
        std::erase_if(type_tokens, is_pipe);

        std::vector<constructs::LSP::types> types(type_tokens.size());
        std::transform(type_tokens.begin(), type_tokens.end(), types.begin(),
                       [](Token& token){
                           assert(token.kind == Identifier && "identifier expected");
                           assert(!token.content.empty() && "content expected");
                           return from_string(token.content);
                       });
        constructs::sum_t sum{
            .variant = types
        };
        type.type = std::move(sum);
        field.type = std::move(type);
        return field;
    }

    assert(false && "Magda missed a case in parsing :P");

    field.type = std::move(type);
    return field;
}

constructs::interface_t TSAssembler::get_parsta() {
    using enum parser::TokenType;

    constructs::interface_t result;
    Token& t = tokenizer.next();
    assert(t.kind == Keyword && t.content == "interface" && "keyword expected");
    Token& ident = tokenizer.next();
    assert(ident.kind == Identifier && "identifier expected");

    result.ident = ident.content;

    if (!unresolved_identifiers.empty()) {
        std::erase_if(unresolved_identifiers, [&](constructs::identifier_t ident) {
            return ident == result.ident;
        });
    }

    if (tokenizer.peek().kind == Keyword && tokenizer.peek().content == "extends") {
        tokenizer.skip();
        assert(tokenizer.peek().kind == Identifier && "identifier expected");
        result.inherits = tokenizer.next().content;
    }

    assert(tokenizer.next().kind == LBrace && "Left brace expected");
    while (tokenizer.peek().kind != RBrace) {
        result.fields.push_back(field_parser(unresolved_identifiers, tokenizer));
    }
    assert(tokenizer.next().kind == RBrace);
    return result;
}

}
