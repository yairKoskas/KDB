#ifndef __DEBUGEE_H
#define __DEBUGEE_H

#include <utility>
#include "elf/ELF.hpp"
#include "DebuggeeExceptions.hpp"

class Debuggee {
private:
    ELF* m_elf;
    pid_t m_tracee;
public:
    explicit Debuggee(ELF* elf) : m_elf(elf), m_tracee(-1) {};
    explicit Debuggee(std::string path) : m_elf(new ELF(path)), m_tracee(-1) {};
    explicit Debuggee(char* path) : m_elf(new ELF(std::string(path))), m_tracee(-1) {};
    ELF* getELF() const { return this->m_elf; };
    pid_t getPID() const { return this->m_tracee; };
    void setPID(pid_t pid) { this->m_tracee = pid; };
};

#endif