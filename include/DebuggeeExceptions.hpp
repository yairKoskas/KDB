//
// Created by yairko on 12/01/2022.
//

#ifndef KDB_DEBUGGEEEXCEPTIONS_HPP
#define KDB_DEBUGGEEEXCEPTIONS_HPP


#include <stdexcept>

class DebugeeNotRunning : public std::runtime_error {
public:
    explicit DebugeeNotRunning(char const* const message="Debuggee not running") noexcept : std::runtime_error(message) {};
    char const* what() const noexcept override {
        return std::runtime_error::what();
    }
};

#endif
