//
// Created by yairko on 12/01/2022.
//

#ifndef KDB_ELFTYPES_HPP
#define KDB_ELFTYPES_HPP

enum class Bits {
    BITS_32 = 1,
    BITS_64
};

enum class Endianness {
    ENDIAN_BIG,
    ENDIAN_LITTLE
};

enum class OSABI {
    OSABI_NONE = 0,
    OSABI_SYSV = 0,
    OSABI_HPUX,
    OSABI_NETBSD,
    OSABI_LINUX,
    OSABI_SOLARIS,
    OSABI_IRIX,
    OSABI_FREEBSD,
    OSABI_TRU64,
    OSABI_ARM,
    OSABI_STANDALONE
};

enum class Elftype {
    NONE,
    REL,
    EXEC,
    DYN,
    CORE
};

enum class Elfmachine {
    NONE,
    M32,
    SPARC,
    I386,
    M68K,
    M88K,
    I860,
    MIPS,
    PARISC,
    SPARC32PLUS,
    PPC,
    PPC64,
    S390,
    ARM,
    SH,
    SPARC9,
    IA_64,
    X86_64,
    VAX
};
typedef uint64_t byte8_t;
typedef uint32_t byte4_t;
typedef uint16_t yte2_t;
typedef uint8_t byte_t;

#endif //KDB_ELFTYPES_HPP
