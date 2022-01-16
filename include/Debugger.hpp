#ifndef __DEBUGGER_H
#define __DEBUGGER_H

#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/ptrace.h>

#include "elf/ELF.hpp"
#include "types.hpp"
#include "Debuggee.hpp"

class Debugger {
private:
    typedef struct {
        address addr;
        byte8_t original_data;
    } Breakpoint;
    Debuggee* m_debuggee;
    std::vector<Breakpoint*> m_breakpoints;
public:
    enum class DebuggeeStatus {
        RUNNING,
        STOPPED,
        EXITED
    };

    explicit Debugger(Debuggee* debugee) : m_debuggee(debugee) {};
    quadword_t getQwordAtAddress(const address& addr) const;
    doubleword_t getDwordAtAddress(const address& addr) const;
    word_t getWordAtAddress(const address& addr) const;
    byte_t getByteAtAddress(const address& addr) const;
    void singleStep();
    void continueProcess();
    void run();
    Debuggee* getDebuggee() { return this->m_debuggee; };
    std::map<std::string, quadword_t> getRegisters();
    void addBreakpoint(address addr);
    void exit();
};

#endif