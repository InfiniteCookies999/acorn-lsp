#include <fstream>
#include <iostream>

#include "parser.h"

const std::string filename = "lsp_defs.ts";

int main() {
    parser::TSTokenizer tokenizer(filename);

    std::cout << sizeof(parser::Token) << std::endl;

    tokenizer.peek().print(); // interface
    tokenizer.next().print(); // interface
    tokenizer.peek().print(); // Message
    tokenizer.next().print(); // Message

    std::cout << "===" << std::endl;
    tokenizer[0].print(); // interface
    tokenizer[1].print(); // Message
    tokenizer[2].print(); // {
    tokenizer[1].print(); // Message
    tokenizer[0].print(); // interface
}
