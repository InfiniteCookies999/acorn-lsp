#include <fstream>
#include <iostream>

#include "parser.h"

const std::string filename = "lsp_defs.ts";

int main() {
    parser::TSTokenizer tokenizer(filename);

    for (std::size_t i = 0; i < 25; ++i)
        tokenizer.get_next_token().print();
}
