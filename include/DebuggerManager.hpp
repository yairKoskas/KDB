//
// Created by yairko on 15/01/2022.
//

#ifndef KDB_DEBUGGERMANAGER_HPP
#define KDB_DEBUGGERMANAGER_HPP

#include "Debugger.hpp"

class DebuggerManager {
private:
    typedef void (*unary_debugger_func)(Debugger&);
    typedef void (*binary_debugger_func)(Debugger&, address addr);

    Debugger* m_debugger;
    std::map<std::string, unary_debugger_func> m_unaryCommands;
    std::map<std::string, binary_debugger_func> m_binaryCommands;
    bool is_running;

public:

    explicit DebuggerManager(Debugger* debugger);
    void execute(const std::string& command);
    Debugger::DebuggeeStatus checkStatus();
    void setRunning(bool b);

    bool isRunning();
};


#endif //KDB_DEBUGGERMANAGER_HPP
