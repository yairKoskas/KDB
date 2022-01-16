    //
// Created by yairko on 14/01/2022.
//

#ifndef KDB_ELFPROGRAMHEADER_HPP
#define KDB_ELFPROGRAMHEADER_HPP

#include <elf.h>
#include <fstream>

#include "ElfTypes.hpp"


class ElfProgramHeader {
private:
    typedef union {
        Elf32_Phdr m_header32;
        Elf64_Phdr m_header64;
    } u_programHeader;
    u_programHeader* m_header;
    Bits bits;
public:
    explicit ElfProgramHeader(Bits bits, std::ifstream& file);
    explicit ElfProgramHeader(Bits bits, std::ifstream& file, uint32_t offset);
};


#endif //KDB_ELFPROGRAMHEADER_HPP
