#include <unistd.h>
#include <sys/user.h>
#include <iostream>
#include "Debugger.hpp"

quadword_t Debugger::getQwordAtAddress(const address& addr) const {
    if (this->m_debuggee->getPID() == -1) {
        throw DebugeeNotRunning();
    }
    return ptrace(PTRACE_PEEKDATA, this->m_debuggee->getPID(), addr, NULL);
}

doubleword_t Debugger::getDwordAtAddress(const address& addr) const {
    quadword_t data = this->getQwordAtAddress(addr);
    return (doubleword_t) data;
}

word_t Debugger::getWordAtAddress(const address& addr) const {
    quadword_t data = this->getQwordAtAddress(addr);
    return (word_t) data;
}

byte_t Debugger::getByteAtAddress(const address& addr) const {
    quadword_t data = this->getQwordAtAddress(addr);
    return (byte_t) data;
}

void Debugger::singleStep() {
    if (this->m_debuggee->getPID() == -1) {
        throw DebugeeNotRunning();
    }
    ptrace(PTRACE_SINGLESTEP, this->m_debuggee->getPID(), NULL, NULL);
}

void Debugger::continueProcess() {
    if (this->m_debuggee->getPID() == -1) {
        throw DebugeeNotRunning();
    }
    ptrace(PTRACE_CONT, this->m_debuggee->getPID(), NULL, NULL);
}

void Debugger::run() {
    pid_t pid = fork();
    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        char arg0[] = "/home/yairko/Desktop/Programming/KDB/tests/bin/echo1";
        char* const args[] = {arg0, nullptr};
        execvp("./tests/bin/echo1", args);
    }
    this->m_debuggee->setPID(pid);
    // set all pending breakpoints
    for (auto & bp : this->m_breakpoints) {
        address aligned_address = bp->addr & (-8);
        byte8_t data;
        while((data = ptrace(PTRACE_PEEKDATA, pid, (long) aligned_address, NULL)) == (byte8_t) -1) {};
        bp->original_data = data;
        std::cout << data << std::endl;
        const uint8_t offset = bp->addr & 7;
        //data, offset, 0xcc
        const int mask = 0xff << (offset << 3);
        const int shift = (0xcc << (offset << 3));
        data = (~mask & data) | shift;
        ptrace(PTRACE_POKEDATA, pid, aligned_address, data);
    }

}

std::map<std::string, quadword_t> Debugger::getRegisters() {
    if (this->m_debuggee->getPID() == -1) {
        throw DebugeeNotRunning();
    }
    std::map<std::string, quadword_t> regs;
    struct user_regs_struct regs_struct{};
    ptrace(PTRACE_GETREGS, this->m_debuggee->getPID(), NULL, &regs_struct);
    regs["rax"] = regs_struct.rax;
    regs["rbx"] = regs_struct.rbx;
    regs["rcx"] = regs_struct.rcx;
    regs["rdx"] = regs_struct.rdx;
    regs["rdi"] = regs_struct.rdi;
    regs["rsi"] = regs_struct.rsi;
    regs["rbp"] = regs_struct.rbp;
    regs["rsp"] = regs_struct.rsp;
    regs["rip"] = regs_struct.rip;
    regs["r8"] = regs_struct.r8;
    regs["r9"] = regs_struct.r9;
    regs["r10"] = regs_struct.r10;
    regs["r11"] = regs_struct.r11;
    regs["r12"] = regs_struct.r12;
    regs["r13"] = regs_struct.r13;
    regs["r14"] = regs_struct.r14;
    regs["r15"] = regs_struct.r15;
    return regs;
}

void Debugger::addBreakpoint(address addr) {
    auto* bp = new Breakpoint();
    bp->addr = addr;
    this->m_breakpoints.push_back(bp);
}

void Debugger::exit() {
    ptrace(PTRACE_KILL, this->getDebuggee()->getPID(), NULL, NULL);
    std::exit(1);
}

