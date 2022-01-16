#include "elf/ElfProgramHeader.hpp"

ElfProgramHeader::ElfProgramHeader(Bits bits, std::ifstream& file) {
    this->bits = bits;
    this->m_header = new u_programHeader();
    if (bits == Bits::BITS_32) {
        file.read((char*) &this->m_header->m_header32, sizeof(Elf32_Phdr));
    } else {
        file.read((char*) &this->m_header->m_header64, sizeof(Elf64_Phdr));
    }
}

ElfProgramHeader::ElfProgramHeader(Bits bits, std::ifstream &file, uint32_t offset) {
    file.seekg(offset);
    new (this) ElfProgramHeader(bits, file);
}
