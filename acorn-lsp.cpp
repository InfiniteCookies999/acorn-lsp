#include <format>
#include <iostream>

#include "logger.h"

static Logger logger;

static std::string make_message(const std::string& message) {
    return std::format("Content-Length:{}\r\n\r\n{}", message.size(), message);
}

int main() {
    //std::cout << make_message("{\"params\":{\"code\":420,\"message\":\"meow\"}}") << '\n';
    
    if (!logger.open()) {
        // Well... what are we suppose to do?
        return 1;
    }

    logger.info("{} {}", "hello", 513412);
    logger.warn("{} {}", "hello", 513412);
    logger.error("{} {}", "hello", 513412);
    
    //make_message("{}")

    return 0;
}
