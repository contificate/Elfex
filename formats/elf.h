#ifndef ELF_H
#define ELF_H

#include <cstdint>
#include <iostream>

namespace efx {

#define ELF_MAGIC \
  "\x7f"          \
  "ELF"

enum class ElfEndian : uint8_t { LITTLE = 1, BIG };

enum class ElfClass : uint8_t { ELF_32 = 1, ELF_64 };

enum class ElfType : uint16_t { RELOCATABLE = 1, EXECUTABLE, SHARED, CORE };

enum class ElfMachine {
  NONE = 0x00,
  SPARC = 0x02,
  X86 = 0x03,
  MIPS = 0x08,
  PPC = 0x14,
  S390 = 0x16,
  ARM = 0x28,
  SUPERH = 0x2a,
  IA64 = 0x32,
  X86_64 = 0x3e,
  AARCH64 = 0xb7,
  RISCV = 0XF3
};

#define EI_NIDENT 0x10
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7

struct ElfHeader {
  uint8_t e_ident[EI_NIDENT];
  ElfType e_type;
  ElfMachine e_machine;
  uint32_t e_version;  // 1 for original ELF

  // dependent on EI_CLASS
  uint64_t e_entry;
  uint64_t e_phoff;
  uint64_t e_shoff;

  uint32_t e_flags;
  uint16_t e_ehsize;
  uint16_t e_phentsize;
  uint16_t e_phnum;
  uint16_t e_shentsize;
  uint16_t e_shnum;
  uint16_t e_shstrndx;

  bool IsLittleEndian() {
    return (e_ident[EI_DATA] == static_cast<uint8_t>(ElfEndian::LITTLE));
  }

  bool Is32Bit() {
    return (e_ident[EI_CLASS] == static_cast<uint8_t>(ElfClass::ELF_32));
  }
};

struct ElfSection {
    uint32_t sh_name; // relative offset string table
    uint32_t sh_type; // type of entry
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;

    friend std::ostream& operator<<(std::ostream& os, const ElfSection& s){
        os << "sh_name: " << s.sh_name << '\n';
        os << "sh_type: " << s.sh_type << '\n';
        os << "sh_flags: " << s.sh_flags << '\n';
        os << "sh_addr: " <<  s.sh_addr<< '\n';
        os << "sh_offset: " << s.sh_offset << '\n';
        os << "sh_size: " << s.sh_size << '\n';
        os << "sh_addralign: " << s.sh_addralign << '\n';

        return os;
    }
};

}  // namespace efx

#endif  // ELF_H
