// vim: ft=cpp
#include <ostream>
#include <string>
#include <variant>
#include <fstream>
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
inline std::ostream &operator<<(std::ostream &s, const TokenType &tt)
{
    std::string word;
    switch (tt) {
    case TokenType::Colon: word = "colon"; break;
    case TokenType::Comment: word = "comment"; break;
    case TokenType::LBrace: word = "left brace"; break;
    case TokenType::Pipe: word = "pipe"; break;
    case TokenType::QuestionMark: word = "question mark"; break;
    case TokenType::RBrace: word = "right brace"; break;
    case TokenType::Semicolon: word = "semicolon"; break;
    case TokenType::Identifier: word = "identifier"; break;
    case TokenType::Keyword: word = "keyword"; break;
    case TokenType::EOFToken: word = "end of file"; break;
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
        std::cout << "\tkind: " << kind << "\n";
        std::cout << "\tcontent: "  << content << "\n";
        std::cout << "}" << std::endl;
    }
};

class TSTokenizer {
    std::fstream file;

    void skip_comment(char &);

  public:
    TSTokenizer(std::string);
    Token get_next_token();
};

}
