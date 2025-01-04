#include <format>
#include <iostream>
#include <sstream>

// interface ResponseError {
//  code: integer;
//  message: string;
// }

std::string make_message(const std::string& message) {
    return std::format("Content-Length:{}\r\n\r\n{}", message.size(), message);
}

int main() {
    std::cout << make_message("{\"params\":{\"code\":420,\"message\":\"meow\"}}") << '\n';
    // send on stdout

    return 420;
}
