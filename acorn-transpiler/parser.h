// vim: ft=cpp

#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include <ostream>
#include <variant>
#include <fstream>
#include <optional>
#include <iostream>

namespace parser {

enum class TokenType {
    Colon,
    Comment,
    LBrace,
    Pipe,
    QuestionMark,
    RBrace,
    Semicolon,
    Identifier,
    Keyword,
    EOFToken
};

// TODO: debug only *remove me*
inline std::ostream& operator<<(std::ostream& s, const TokenType& tt)
{
    std::string word;
    switch (tt) {
        case TokenType::Colon:        word = "colon";         break;
        case TokenType::Comment:      word = "comment";       break;
        case TokenType::LBrace:       word = "left brace";    break;
        case TokenType::Pipe:         word = "pipe";          break;
        case TokenType::QuestionMark: word = "question mark"; break;
        case TokenType::RBrace:       word = "right brace";   break;
        case TokenType::Semicolon:    word = "semicolon";     break;
        case TokenType::Identifier:   word = "identifier";    break;
        case TokenType::Keyword:      word = "keyword";       break;
        case TokenType::EOFToken:     word = "end of file";   break;
    }
    s << word;
    return s;
}

struct Token {
    TokenType kind;
    std::string content;

    // TODO: debug only *remove me*
    void print() {
        std::cout << "{\n";
        std::cout << "\tkind: "     << kind << "\n";
        std::cout << "\tcontent: "  << content << "\n";
        std::cout << "}"            << std::endl;
    }
};

class TSTokenizer {
    std::fstream file;
    std::size_t position;
    std::vector<Token> tokens;

    void skip_comment(char&);
    Token get_next_token();

  public:
    TSTokenizer(const std::string&);

    Token& operator[](std::size_t);
    Token& next();
    Token& peek();
    void clear();
    void skip();
};

namespace constructs {

using identifier_t = std::string;
using inherits_t = std::optional<identifier_t>;

namespace LSP {

struct integer {
    std::int32_t data;
    operator std::string () const {
        return "std::int32_t";
    }
};

struct uinteger {
    std::uint32_t data;
    operator std::string () const {
        return "std::uint32_t";
    }
};

struct null {
    operator std::string () const {
        return "std::nullopt_t";
    }
};

struct string {
    std::string data;
    operator std::string () const {
        return "std::string";
    }
};

struct decimal {
    double data;
    operator std::string () const {
        return "double";
    }
};

struct boolean {
    bool data;
    operator std::string () const {
        return "bool";
    }
};

struct Any;

struct Object {
    std::unordered_map<std::string, Any> data;
    operator std::string () const {
        return "std::unordered_map<std::string, parser::constructs::LSP::Any>";
    }
};

struct Array {
    std::vector<Any> data;
    operator std::string () const {
        return "std::vector<parser::constructs::LSP::Any>";
    }
};

struct Any {
  public:
    using Type = std::variant<
        Object,
        Array,
        string,
        integer,
        uinteger,
        decimal,
        boolean,
        null
    >;

    Type data;

    Any() = default;

    template<typename T>
    Any(T&& t) : data(std::forward<T>(t)){}

    operator std::string () const {
        return "parser::constructs::LSP::Any";
    };
};

using types = std::variant<
    LSP::boolean,
    LSP::string,
    LSP::integer,
    LSP::uinteger,
    LSP::null,
    LSP::decimal,
    LSP::Array,
    LSP::Object,
    LSP::Any
>;

};

struct sum_t {
    std::vector<LSP::types> variant;

    operator std::string () const {
        std::string result { "std::variant<" };
        for (const auto& t: variant) {
            result += std::visit([](const auto& value) -> std::string {
                return static_cast<std::string>(value);
            }, t);
            result += ',';
        }
        result.back() = '>';
        return result;
    }
};

using type_variant = std::variant<
    LSP::boolean,
    LSP::string,
    LSP::integer,
    LSP::uinteger,
    LSP::null,
    LSP::decimal,
    LSP::Array,
    LSP::Object,
    LSP::Any,
    sum_t
>;

struct type_t {
    bool is_optional;
    type_variant type;

    operator std::string () const {
        std::string type_str = std::visit([](const auto& var) -> std::string {
            return static_cast<std::string>(var);
        }, type);

        if (is_optional) {
            std::string result { "std::optional<" };
            result += type_str;
            result += '>';
            return result;
        }

        return type_str;
    }
};

struct field_t {
    identifier_t ident;
    type_t type;

    operator std::string () const {
        std::string result { "\t" };
        result += type;
        result += ' ';
        result += ident;
        result += ";\n";
        return result;
    }
};

struct interface_t {
    identifier_t ident;
    inherits_t inherits;
    std::vector<field_t> fields;

    operator std::string () const {
        std::string result { "struct " };
        result += ident;
        if (inherits) {
            result += " : public ";
            result += *inherits;
        }
        result += " {\n";
        for (const auto& field: fields) {
            result += field;
        }
        result += "};\n";
        return result;
    }
};

};

class TSAssembler {
    TSTokenizer tokenizer;

  public:
    TSAssembler(const std::string& filename);

    constructs::interface_t get_parsta();
};

}
