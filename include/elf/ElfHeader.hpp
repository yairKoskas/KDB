//
// Created by yairko on 12/01/2022.
//

#ifndef KDB_ELFHEADER_HPP
#define KDB_ELFHEADER_HPP

#include <elf.h>
#include <string>


#include "elf/ElfTypes.hpp"

class ElfHeader {
private:
    union {
        Elf32_Ehdr m_header32;
        Elf64_Ehdr m_header64;
    } m_header{};
    uint8_t bits;
public:
    explicit ElfHeader(const std::string& path);
    explicit ElfHeader(std::ifstream& file);
    Bits getBits() const;
    Endianness getEndianness() const;
    OSABI getOSABI() const;
    Elftype getType() const;
    Elfmachine getMachine() const;
    byte8_t getEntry() const;
    byte8_t getProgramHeaderOffset() const;
    byte8_t getSectionHeaderOffset() const;
    uint16_t getNumberOfProgramHeaders() const;
    uint16_t getNumberOfSectionHeaders() const;

    uint16_t getStrtabOffset();
};

#endif //KDB_ELFHEADER_HPP
