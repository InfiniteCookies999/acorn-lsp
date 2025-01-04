#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <filesystem>

class Logger {
public:

    bool open();

    void info(const char* message);

    void warn(const char* message);

    void error(const char* message);

private:
    std::ofstream ostream;

    void write(const char* message, const char* type_str);

};

#endif // LOGGER_H