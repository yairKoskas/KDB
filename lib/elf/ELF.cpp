#include <cstring>
#include <iostream>
#include "elf/ELF.hpp"


void loadStrtab(std::map<uint32_t, std::string> &strtab, std::ifstream& file, uint32_t size) {
    strtab[0] = std::string("");
    uint32_t total_size = 1;
    uint16_t num_of_strings = 1;
    uint32_t index = 1;
    while (total_size < size) {
        char temp[256];
        file.getline((char*) temp, 256, '\0');
        if (strlen(temp) == 0) {
            break;
        }
        strtab[index] = std::string(temp);
        index += strtab[index].size() + 1;
        total_size += strtab[index].size();
        num_of_strings++;
    }
}

ELF::ELF(const std::string &path) {
    this->m_path = path;
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);
    this->m_header = new ElfHeader(file);
    this->m_programHeaders = std::vector<ElfProgramHeader*>();
    this->m_sectionHeaders = std::vector<ElfSectionHeader*>();

    file.seekg((long) this->getHeader()->getProgramHeaderOffset(), std::ios::beg);
    for (uint16_t i = 0; i < this->getHeader()->getNumberOfProgramHeaders(); ++i) {
        this->m_programHeaders.push_back(new ElfProgramHeader(this->getHeader()->getBits(), file));
    }
    file.seekg((long) this->getHeader()->getSectionHeaderOffset(), std::ios::beg);
    for (uint16_t i = 0; i < this->getHeader()->getNumberOfSectionHeaders(); ++i) {
        this->m_sectionHeaders.push_back(new ElfSectionHeader(this->getHeader()->getBits(), file));
    }
    ElfSectionHeader* &strtab = this->m_sectionHeaders[this->getHeader()->getStrtabOffset()];
    file.seekg((long) strtab->getOffset() + 1);
    // also allocates each string
    loadStrtab(this->m_strtable, file, strtab->getSectionSize());
}

ElfHeader *ELF::getHeader() {
    return this->m_header;
}

ELF::ELF(std::ifstream &file) {
    this->m_header = new ElfHeader(file);
}

ElfSectionHeader *ELF::getSection(const std::string& name) {
    // mistake, offset is IN the strtab not index in the strtab
    for (uint16_t i = 0; i < this->getHeader()->getNumberOfSectionHeaders(); ++i) {
        ElfSectionHeader* curr = this->m_sectionHeaders[i];
        if (this->m_strtable[curr->getNameOffset()] == name) {
            return this->m_sectionHeaders[i];
        }
    }
    return nullptr;
}

std::vector<ElfSectionHeader *> ELF::getSectionHeaders() {
    return this->m_sectionHeaders;
}

std::string ELF::getPath() {
    return this->m_path;
}
