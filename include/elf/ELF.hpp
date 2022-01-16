#ifndef __ELF_H
#define __ELF_H

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <elf.h>

#include "elf/ElfHeader.hpp"
#include "elf/ElfSectionHeader.hpp"
#include "ElfProgramHeader.hpp"

class ELF {
private:
    ElfHeader* m_header;
    std::vector<ElfProgramHeader*> m_programHeaders;
    std::vector<ElfSectionHeader*> m_sectionHeaders;
    std::map<uint32_t, std::string> m_strtable;
    std::string m_path;
public:
    explicit ELF(const std::string& path);
    explicit ELF(std::ifstream& file);
    ElfHeader* getHeader();
    std::vector<ElfSectionHeader*> getSectionHeaders();
    ElfSectionHeader* getSection(const std::string& name);
    std::string getPath();
};

#endif