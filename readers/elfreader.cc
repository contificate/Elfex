#include "elfreader.h"

namespace efx {

ElfReader::ElfReader(ElfReader&& other) { *this = std::move(other); }

ElfReader& ElfReader::operator=(ElfReader&& other) {
  if (this != &other) {
    reader_ = std::move(other.reader_);
    other.reader_ = nullptr;
  }

  return *this;
}

/**
 * Create an ELF reader from a file source
 * @param fileLocation path of the ELF file
 * @return unique poointer instance of an ElfReader; the moveable-only nature is
 * intrinsic
 */
UPtrElfReader ElfReader::CreateFromFile(const std::string& fileLocation) {
  try {
    auto fileReader = std::make_unique<FileReader>(fileLocation);

    // TODO: read working header (obscurecolin)

    // cannot use std::make_unique, it's not friends with this class
    return UPtrElfReader(new ElfReader(std::move(fileReader)));
  } catch (const std::runtime_error& kEx) {
    throw;
  }

  return nullptr;
}

/**
 * Create an ELF reader from a mapped memory source (depending on size of file,
 * possibly mmap'ed on linux)
 * @param buffer location of mapped ELF file
 * @param len length of entire ELF file
 * @return unique poointer instance of an ElfReader; the moveable-only nature is
 * intrinsic
 */
UPtrElfReader ElfReader::CreateFromMemory(const uint8_t* const buffer,
                                          const std::streamsize len) {
  try {
    auto memReader = std::make_unique<MemReader>(buffer, len);

    // TODO: read working header (obscurecolin)

    // cannot use std::make_unique, it's not friends with this class
    return UPtrElfReader(new ElfReader(std::move(memReader)));
  } catch (const std::runtime_error& kEx) {
    throw;
  }

  return nullptr;
}

ElfReader::ElfReader(std::unique_ptr<BinaryReader> reader)
    : reader_{std::move(reader)} {}

}  // namespace efx
