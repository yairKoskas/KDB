//
// Created by yairko on 12/01/2022.
//

#ifndef KDB_ELFSECTIONHEADER_HPP
#define KDB_ELFSECTIONHEADER_HPP

#include <elf.h>
#include <string>
#include "ElfTypes.hpp"


class ElfSectionHeader {
private:
    typedef union {
        Elf32_Shdr m_header32;
        Elf64_Shdr m_header64;
    } u_sectionHeader;
    u_sectionHeader* m_header;
    Bits bits;
public:
    explicit ElfSectionHeader(Bits bits, const std::string& path, uint32_t offset);
    explicit ElfSectionHeader(Bits bits, std::ifstream& file, uint32_t offset);
    explicit ElfSectionHeader(Bits bits, std::ifstream& file);
    uint32_t getNameOffset();
    uint32_t getSectionSize();
    uint64_t getOffset();

    byte8_t getAddress();
};

#endif //KDB_ELFSECTIONHEADER_HPP
