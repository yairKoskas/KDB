#include <iostream>
#include <vector>
#include "DebuggerManager.hpp"

std::vector<std::string> split(std::string s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        tokens.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);
    return tokens;
}

void singleStep(Debugger& dbg) {
    dbg.singleStep();
}

void dumpRegisters(Debugger& dbg) {
    auto regs = dbg.getRegisters();
    for (auto & reg : regs) {
        std::cout << reg.first << ':' << reg.second << std::endl;
    }
}

void run(Debugger& dbg) {
    dbg.run();
    dbg.continueProcess();
}

void continueRun(Debugger& dbg) {
    dbg.continueProcess();
}

void setBreakpoint(Debugger& dbg, address addr) {
    dbg.addBreakpoint(addr);
}

void exitDebugger(Debugger& dbg) {
    dbg.exit();
}

void examineMemory(Debugger& dbg, address addr) {
    if (dbg.getDebuggee()->getPID() == -1) {
        for (ElfSectionHeader* section : dbg.getDebuggee()->getELF()->getSectionHeaders()) {
            byte8_t virt_addr = section->getAddress();
            uint32_t size = section->getSectionSize();
            if (virt_addr < addr && addr < virt_addr + size) {
                // data is in this section
                byte8_t offset = section->getOffset();
                uint32_t offsetInSection = addr - virt_addr;
                std::ifstream file;
                file.open(dbg.getDebuggee()->getELF()->getPath());
                file.seekg((long)offset+offsetInSection, std::ios::beg);
                byte8_t temp;
                file.read((char*)&temp, sizeof(temp));
                std::cout << temp << std::endl;
                file.close();
            }
        }
    } else {
        byte8_t temp;
        temp = ptrace(PTRACE_PEEKDATA, dbg.getDebuggee()->getPID(), addr, NULL);
        std::cout << temp << std::endl;
    }
}

DebuggerManager::DebuggerManager(Debugger *debugger) {
    this->m_debugger = debugger;
    this->is_running = false;
    this->m_unaryCommands["si"] = singleStep;
    this->m_unaryCommands["regs"] = dumpRegisters;
    this->m_unaryCommands["run"] = run;
    this->m_unaryCommands["exit"] = exitDebugger;
    this->m_unaryCommands["c"] = continueRun;
    this->m_binaryCommands["b"] = setBreakpoint;
    this->m_binaryCommands["x"] = examineMemory;
}

void DebuggerManager::execute(const std::string& command) {
    if (split(command, " ").size() == 1) {
        this->m_unaryCommands[command](*this->m_debugger);
    } else {
        auto parsed = split(command, " ");
        std::string value = parsed[1];
        address addr;
        if (value.substr(0, 2) == "0x") {
            value.erase(0, 2);
            addr = std::stol(value, 0, 16);
        } else {
            addr = std::stol(value);
        }
        this->m_binaryCommands[parsed[0]](*this->m_debugger, addr);
    }

}

void DebuggerManager::setRunning(bool b) {
    this->is_running = b;
}

bool DebuggerManager::isRunning() {
    return this->is_running;
}

Debugger::DebuggeeStatus DebuggerManager::checkStatus() {
    if (this->is_running) {
        return Debugger::DebuggeeStatus::RUNNING;
    }

}
