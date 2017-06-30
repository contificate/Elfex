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

uint64_t ElfReader::entry() const { return working_hdr_->e_entry; }

ElfReader::ElfReader(std::unique_ptr<BinaryReader> reader)
    : reader_{std::move(reader)} {
  working_hdr_ = std::unique_ptr<ElfHeader>(new ElfHeader);

  // read (working) header
  reader_->Seek(0);
  reader_->Read(reinterpret_cast<char*>(&working_hdr_->e_ident), EI_NIDENT);

  working_hdr_->e_type = static_cast<ElfType>(reader_->Read<uint16_t>());
  working_hdr_->e_machine = static_cast<ElfMachine>(reader_->Read<uint16_t>());
  working_hdr_->e_version = reader_->Read<uint32_t>();

  if (working_hdr_->Is32Bit()) {
    working_hdr_->e_entry = reader_->Read<uint32_t>();
    working_hdr_->e_phoff = reader_->Read<uint32_t>();
    working_hdr_->e_shoff = reader_->Read<uint32_t>();
  } else {
    working_hdr_->e_entry = reader_->Read<uint64_t>();
    working_hdr_->e_phoff = reader_->Read<uint64_t>();
    working_hdr_->e_shoff = reader_->Read<uint64_t>();
  }

  working_hdr_->e_flags = reader_->Read<uint32_t>();

  uint16_t* const latter = &working_hdr_->e_ehsize;

  // the latter 6 fields are heterogenous
  for (unsigned i = 0; i < 6; i++) *(latter + i) = reader_->Read<uint16_t>();
}

const std::unique_ptr<ElfHeader>& ElfReader::header() const {
  return working_hdr_;
}

}  // namespace efx
