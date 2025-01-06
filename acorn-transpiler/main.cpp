#include <fstream>
#include <iostream>

#include "parser.h"

const std::string filename = "lsp_defs.ts";

namespace LSP = parser::constructs::LSP;
namespace con = parser::constructs;

int main() {
    parser::TSAssembler assembler{"lsp_defs.ts"};
    std::cout << (std::string)assembler.get_parsta() << std::endl;
    std::cout << (std::string)assembler.get_parsta() << std::endl;
    std::cout << (std::string)assembler.get_parsta() << std::endl;
}
