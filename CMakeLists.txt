cmake_minimum_required(VERSION 3.21)
project(KDB)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

add_executable(KDB
        include/Debugger.hpp
        include/Debuggee.hpp
        include/elf/ELF.hpp
        include/types.hpp
        lib/Debugger.cpp
        src/main.cpp include/DebuggeeExceptions.hpp lib/elf/ELF.cpp include/elf/ElfHeader.hpp include/elf/ElfTypes.hpp include/elf/ElfSectionHeader.hpp lib/elf/ElfHeader.cpp include/elf/ElfProgramHeader.hpp lib/elf/ElfSectionHeader.cpp lib/elf/ElfProgramHeader.cpp include/DebuggerManager.hpp lib/DebuggerManager.cpp)

target_link_libraries(KDB
        pthread)