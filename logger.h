#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <filesystem>
#include <format>

class Logger {
public:

    bool open();

    void info(const char* message);
    void info(const std::string& message);

    template<typename... T>
    void info(const char* fmt, T&&... args) {
        write_fmt(1, "INFO", fmt, std::forward<T>(args)...);
    }

    void warn(const char* message);
    void warn(const std::string& message);

    template<typename... T>
    void warn(const char* fmt, T&&... args) {
        write_fmt(1, "WARN", fmt, std::forward<T>(args)...);
    }

    void error(const char* message);
    void error(const std::string& message);

    template<typename... T>
    void error(const char* fmt, T&&... args) {
        write_fmt(0, "ERROR", fmt, std::forward<T>(args)...);
    }

private:
    std::ofstream ostream;

    template<typename... T>
    void write_fmt(int pad, const char* type_str, const char* fmt, T&&... args) {
        write_timestamp(pad, type_str);
        ostream << std::vformat(fmt, std::make_format_args(args...)) << "\n";
    }

    void write_timestamp(size_t pad, const char* type_str);
    void write(size_t pad, const char* message, const char* type_str);

};

#endif // LOGGER_H