#ifndef ELFREADER_H
#define ELFREADER_H

#include <cstring>
#include <ios>
#include <map>
#include <memory>
#include <string>

#include "../formats/elf.h"
#include "filereader.h"
#include "memreader.h"

namespace efx {

class ElfReader;
using UPtrElfReader = std::unique_ptr<ElfReader>;

// aliases
using UPtrBinaryReader = std::unique_ptr<BinaryReader>;
using UPtrSection = std::unique_ptr<ElfSection>;
using SectionMap = std::map<uint16_t, UPtrSection>;
using UPtrSectionMap = std::unique_ptr<SectionMap>;
using SectionPair = std::pair<const uint16_t, UPtrSection>;
using UPtrElfHeader = std::unique_ptr<ElfHeader>;
using UPtrStringTable = std::unique_ptr<const char[]>;

class ElfReader {
 public:
  /* implicitly deletes copy constructor(s), this class is only movable to
   * impose
   * ownership limitation */
  ElfReader(ElfReader&& other) noexcept;
  ElfReader& operator=(ElfReader&& other) noexcept;

  static UPtrElfReader CreateFromFile(const std::string& fileLocation);
  static UPtrElfReader CreateFromMemory(const uint8_t* const buffer,
                                        const std::streamsize len);

  // get entry point from working header
  uint64_t entry() const;

  // get section header count
  uint16_t section_count() const;

  /**
   * @return immutable reference to the working header being used by this reader
   * Perhaps improve encapsulation by facading access to this via public methods
   * (obscurecolin)
   */
  const UPtrElfHeader& header() const;
  const UPtrStringTable& string_table() const;

 private:
  // internal reader
  UPtrBinaryReader reader_;

  // ELF specific
  UPtrElfHeader working_hdr_;
  UPtrSectionMap section_map_;
  UPtrStringTable string_table_;

  // feed reader
  ElfReader(std::unique_ptr<BinaryReader> reader);

  // readers for operational dependencies
  void ReadSectionHdrs();
  void ReadStringTable();
};

}  // namespace efx

#endif  // ELFREADER_H
