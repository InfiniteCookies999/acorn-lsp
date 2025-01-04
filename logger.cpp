#include "logger.h"

#include <string>
#include <chrono>

#include "prelude.h"

#if WIN_OS
#include <Windows.h>
#undef min
#undef max
#elif MAC_OS
#include <mach-o/dyld.h>
#endif

// interface ResponseError {
//  code: integer;
//  message: string;
// }

static std::string get_executable_path() {
#if WIN_OS
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length != 0) {
        buffer[length] = '\0';
        return std::string(buffer);
    }
    return "";
#elif MAC_OS
    char buffer[PATH_MAX + 1];
    uint32_t length = PATH_MAX;
    if(!_NSGetExecutablePath(buf, &length)) {
        buffer[length] = '\0';
        return std::string(buffer);
    }
    return "";
#elif UNIX_OS
    char buffer[PATH_MAX + 1];
    ssize_t length = readlink("/proc/self/exe", buffer, PATH_MAX);
    if (length != -1) {
        buffer[length] = '\0';
        return std::string(buffer);
    }
    return "";
#endif
}

bool Logger::open() {

    namespace fs = std::filesystem;
    
    fs::path exe_path(get_executable_path());
    exe_path.remove_filename();

    ostream.open(exe_path / "log.txt", std::ios::out | std::ios::app);
    ostream << "\n...\n";

    if (!ostream) {
        return false;
    }

    return true;
}

void Logger::info(const char* message) {
    write(message, "INFO");
}

void Logger::warn(const char* message) {
    write(message, "WARN");
}

void Logger::error(const char* message) {
    write(message, "ERROR");
}

void Logger::write(const char* message, const char* type_str) {

    auto now = std::chrono::system_clock::now();

    // Convert to system time
    std::time_t sys_time = std::chrono::system_clock::to_time_t(now);
    std::tm timestamp = *std::localtime(&sys_time);

    ostream << "[" << std::put_time(&timestamp, "%Y-%m-%d %H:%M:%S") << "][" << type_str <<  "]: ";
    ostream << message << "\n";
}
