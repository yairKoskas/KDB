#include <cstring>
#include <fstream>

#include "elf/ElfHeader.hpp"


ElfHeader::ElfHeader(const std::string& path) {
    // first file opening is for determining bits
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);
    new (this) ElfHeader(file);
}

Bits ElfHeader::getBits() const {
    return Bits(this->bits);
}

Endianness ElfHeader::getEndianness() const {
    if (this->getBits() == Bits::BITS_32) {
        return Endianness(this->m_header.m_header32.e_ident[EI_DATA]);
    }
    return Endianness(this->m_header.m_header64.e_ident[EI_DATA]);
}

OSABI ElfHeader::getOSABI() const {
    if (this->getBits() == Bits::BITS_32) {
        return OSABI(this->m_header.m_header32.e_ident[EI_OSABI]);
    }
    return OSABI(this->m_header.m_header64.e_ident[EI_OSABI]);
}

Elftype ElfHeader::getType() const {
    if (this->getBits() == Bits::BITS_32) {
        return Elftype(this->m_header.m_header32.e_type);
    }
    return Elftype(this->m_header.m_header64.e_type);
}

Elfmachine ElfHeader::getMachine() const {
    if (this->getBits() == Bits::BITS_32) {
        return Elfmachine(this->m_header.m_header32.e_machine);
    }
    return Elfmachine(this->m_header.m_header64.e_machine);
}

byte8_t ElfHeader::getEntry() const {
    if (this->getBits() == Bits::BITS_32) {
        return this->m_header.m_header32.e_entry;
    }
    return this->m_header.m_header64.e_entry;
}

byte8_t ElfHeader::getProgramHeaderOffset() const {
    if (this->getBits() == Bits::BITS_32) {
        return this->m_header.m_header32.e_phoff;
    }
    return this->m_header.m_header64.e_phoff;
}

byte8_t ElfHeader::getSectionHeaderOffset() const {
    if (this->getBits() == Bits::BITS_32) {
        return this->m_header.m_header32.e_shoff;
    }
    return this->m_header.m_header64.e_shoff;
}

ElfHeader::ElfHeader(std::ifstream &file) {
    unsigned char e_ident[EI_NIDENT];
    file.read((char*) e_ident, EI_NIDENT);
    if (e_ident[EI_CLASS] == 1) {
        this->bits = 1;
    } else if (e_ident[EI_CLASS] == 0) {
        throw std::runtime_error("invalid ELF");
    } else {
        this->bits = 2;
    }
    // copying the already populated e_ident
    memcpy(&this->m_header.m_header32, e_ident, sizeof(e_ident));
    if (this->getBits() == Bits::BITS_32) {
        file.read((char*) &this->m_header.m_header32.e_type, sizeof(Elf32_Ehdr) - EI_NIDENT);
    } else {
        file.read((char*) &this->m_header.m_header64.e_type, sizeof(Elf64_Ehdr) - EI_NIDENT);
    }
}

uint16_t ElfHeader::getNumberOfProgramHeaders() const {
    if (this->getBits() == Bits::BITS_32) {
        return this->m_header.m_header32.e_phnum;
    }
    return this->m_header.m_header64.e_phnum;
}

uint16_t ElfHeader::getNumberOfSectionHeaders() const {
    if (this->getBits() == Bits::BITS_32) {
        return this->m_header.m_header32.e_shnum;
    }
    return this->m_header.m_header64.e_shnum;
}

uint16_t ElfHeader::getStrtabOffset() {
    if (this->getBits() == Bits::BITS_32) {
        return this->m_header.m_header32.e_shstrndx;
    }
    return this->m_header.m_header64.e_shstrndx;
}
