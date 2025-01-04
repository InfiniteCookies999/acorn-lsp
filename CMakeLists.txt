# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("acorn-lsp")

add_executable (acorn-lsp "acorn_lsp.cpp"  "logger.h" "logger.cpp" "prelude.h" "json.hpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET acorn-lsp PROPERTY CXX_STANDARD 20)
endif()

# Include sub-projects.
# add_subdirectory ("acorn-lsp")
