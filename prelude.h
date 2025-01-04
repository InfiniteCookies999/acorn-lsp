#ifndef PRELUDE_H
#define PRELUDE_H

#define WIN_OS  (_WIN32 != 0)
#define MAC_OS  (defined(__APPLE__) && defined(__MACH__))
#define UNIX_OS (defined(__unix__) || defined(__unix) || defined(__linux__) || MAC_OS)

#endif // PRELUDE_H