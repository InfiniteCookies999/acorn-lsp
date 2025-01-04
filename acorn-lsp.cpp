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

    logger.info("pizza!");
    logger.warn("omg we so fucked up");
    logger.error("HOLY FUCKING SHIT THIS IS CRITICAL");

    return 0;
}
