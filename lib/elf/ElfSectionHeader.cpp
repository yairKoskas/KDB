//
// Created by yairko on 14/01/2022.
//

#include <fstream>
#include "elf/ElfSectionHeader.hpp"


ElfSectionHeader::ElfSectionHeader(Bits bits, const std::string &path, uint32_t offset) {
    this->bits = bits;
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);
    new (this) ElfSectionHeader(bits, file, offset);
}

ElfSectionHeader::ElfSectionHeader(Bits bits, std::ifstream& file, uint32_t offset) {
    this->bits = bits;
    file.seekg(offset);
    new (this) ElfSectionHeader(bits, file);
}

ElfSectionHeader::ElfSectionHeader(Bits bits, std::ifstream& file) {
    this->bits = bits;
    this->m_header = new u_sectionHeader();
    if (bits == Bits::BITS_32) {
        file.read((char*) &this->m_header->m_header32, sizeof(Elf32_Shdr));
    } else {
        file.read((char*) &this->m_header->m_header64, sizeof(Elf64_Shdr));
    }
}

uint32_t ElfSectionHeader::getNameOffset() {
    if (bits == Bits::BITS_32) {
        return this->m_header->m_header32.sh_name;
    }
    return this->m_header->m_header64.sh_name;
}

uint32_t ElfSectionHeader::getSectionSize() {
    if (bits == Bits::BITS_32) {
        return this->m_header->m_header32.sh_size;
    }
    return this->m_header->m_header64.sh_size;
}

uint64_t ElfSectionHeader::getOffset() {
    if (bits == Bits::BITS_32) {
        return this->m_header->m_header32.sh_offset;
    }
    return this->m_header->m_header64.sh_offset;
}

byte8_t ElfSectionHeader::getAddress() {
    if (bits == Bits::BITS_32) {
        return this->m_header->m_header32.sh_addr;
    }
    return this->m_header->m_header64.sh_addr;
}
