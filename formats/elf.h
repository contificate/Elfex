#ifndef ELF_H
#define ELF_H

#include <cstdint>

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

}  // namespace efx

#endif  // ELF_H
