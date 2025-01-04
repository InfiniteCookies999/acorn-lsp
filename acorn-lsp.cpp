#include <format>
#include <iostream>
#include <ranges>

#include "logger.h"

#include "json.hpp"

static Logger logger;

static std::string make_message(const std::string& message) {
    return std::format("Content-Length:{}\r\n\r\n{}", message.size(), message);
}

static std::string trim_leading(const std::string& s) {
    auto view = s | std::views::drop_while(isspace);
    std::string result(view.begin(), view.end());
    return result;
}

using json = nlohmann::json;

/**
 * The rootPath of the workspace. Is null
 * if no folder is open.
 *
 * @deprecated in favour of `rootUri`.
 */
static std::optional<std::string> root_path;

/**
 * The rootUri of the workspace. Is null if no
 * folder is open. If both `rootPath` and `rootUri` are set
 * `rootUri` wins.
 *
 * @deprecated in favour of `workspaceFolders`
 */
static std::optional<std::string> root_uri;

/*
 * Read in initial information send by the client.
 * 
 * This function reads in capabilities that the client
 * supports so that when we decide to implement features
 * we may check that the client supports it first.
 */
static void read_initialize(json params) {

    if (params.contains("root_path")) {
        root_path = params["root_path"];
        
        if (root_path.has_value())
            logger.info("root path: {}", root_path.value());
    }

    if (params.contains("root_uri")) {
        root_uri = params["root_uri"];

        if (root_uri.has_value())
            logger.info("root uri: {}", root_path.value());
    }

    // TODO: read in client capabilities

}

static bool read_message() {

    std::string content_length_msg;
    size_t content_length = 0;
    
    std::cin >> content_length_msg;
    if (content_length_msg != "Content-Length:") {
        logger.error("Recieved uknown protocol header for content length: {}", content_length_msg);
        return false;
    }

    std::cin >> content_length;
    std::string content(content_length + 2, ' '); // +2 for \r\n

    std::streamsize total_read = 0;
    std::cin.read(content.data(), static_cast<std::streamsize>(content_length) + 2);

    try {

        auto body = json::parse(trim_leading(content));

        std::string method = body["method"];

        if (method == "initialize") {
            read_initialize(body["params"]);
        }

    } catch (std::exception& e) {
        logger.error("exception info: {}", e.what());
    }

    return true;
}

int main() {
    //std::cout << make_message("{\"params\":{\"code\":420,\"message\":\"meow\"}}") << '\n';
    
    if (!logger.open()) {
        // Well... what are we suppose to do?
        return 1;
    }
    
    if (!read_message()) {
        return 1;
    }

    //make_message("{}");

    return 0;
}
