#include "elfreader.h"

namespace efx {

/*
 * TODO: fix this class, for some reason or another it's not reading correct
 * offsets
 * or at least, from what I've observed, what it's reading doesn't resemble
 * output from readelf. (obscurecolin)
 */

ElfReader::ElfReader(ElfReader&& other) noexcept { *this = std::move(other); }

ElfReader& ElfReader::operator=(ElfReader&& other) noexcept {
  if (this != &other) {
    reader_ = std::move(other.reader_);
    other.reader_ = nullptr;

    working_hdr_ = std::move(other.working_hdr_);
    other.working_hdr_ = nullptr;

    section_map_ = std::move(other.section_map_);
    other.section_map_ = nullptr;

    string_table_ = std::move(other.string_table_);
    other.string_table_ = nullptr;
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
    : reader_{std::move(reader)}, section_map_{nullptr} {
  working_hdr_ = std::unique_ptr<ElfHeader>(new ElfHeader);

  // read (working) header
  reader_->Seek(0);
  reader_->Read(reinterpret_cast<char*>(&working_hdr_->e_ident), EI_NIDENT);

  // check magic
  if (std::strncmp(reinterpret_cast<const char*>(working_hdr_.get()), ELF_MAGIC,
                   4) != 0)
    throw std::runtime_error("ELF magic magic!");

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

  // read section headers
  ReadSectionHdrs();

  // read string table as it's required for mapping every other section
  ReadStringTable();
}

/**
 * @brief ElfReader::ReadSections
 * Read all of the ELF sections.
 */
void ElfReader::ReadSectionHdrs() {
  // if already read, don't read again
  if (section_map_.get()) return;

  // create section map
  section_map_ = std::make_unique<SectionMap>();

  // seek to section header table
  reader_->Seek(working_hdr_->e_shoff);

  for (size_t i = 0; i < working_hdr_->e_shnum; i++) {
    auto section = std::make_unique<ElfSection>();

    section->sh_name = reader_->Read<uint32_t>();
    section->sh_type = static_cast<SectionType>(reader_->Read<uint32_t>());

    const bool k32bit = working_hdr_->Is32Bit();

    if (k32bit) {
      section->sh_flags = reader_->Read<uint32_t>();
      section->sh_addr = reader_->Read<uint32_t>();
      section->sh_offset = reader_->Read<uint32_t>();
      section->sh_size = reader_->Read<uint32_t>();
    } else {
      section->sh_flags = reader_->Read<uint64_t>();
      section->sh_addr = reader_->Read<uint64_t>();
      section->sh_offset = reader_->Read<uint64_t>();
      section->sh_size = reader_->Read<uint64_t>();
    }

    section->sh_link = reader_->Read<uint32_t>();
    section->sh_info = reader_->Read<uint32_t>();

    if (k32bit) {
      section->sh_addralign = reader_->Read<uint32_t>();
      section->sh_entsize = reader_->Read<uint32_t>();
    } else {
      section->sh_addralign = reader_->Read<uint64_t>();
      section->sh_entsize = reader_->Read<uint64_t>();
    }

    // insert into map
    section_map_->insert(SectionPair(i, std::move(section)));
  }
}

void ElfReader::ReadStringTable() {
  if (string_table_.get()) return;

  // locate string table section header
  const auto& kStrSectionHdr = section_map_->at(working_hdr_->e_shstrndx);

  // get string table length
  const uint16_t kSize = kStrSectionHdr->sh_size;

  // allocate string table
  string_table_ = std::make_unique<const char[]>(kSize);

  // seek to address of section in image file
  reader_->Seek(kStrSectionHdr->sh_offset);

  // read section
  reader_->Read(const_cast<char*>(string_table_.get()), kSize);
}

/**
 * @brief ElfReader::ReadTextSection
 * @param len returns length of text section (if not found, len = 0)
 * @return mapped text section
 */
std::unique_ptr<uint8_t[]> ElfReader::ReadTextSection(uint64_t& len) {
  // get section name table
  const char* const kTable = string_table_.get();

  // iterate all sections
  for (const auto& kSection : *section_map_) {
    // get reference to section header
    const auto& hdr = kSection.second;

    // check section (name and type)
    if ((strcmp(".text", (kTable + hdr->sh_name)) == 0) &&
        (hdr->sh_type == SectionType::SHT_PROGBITS)) {
      len = hdr->sh_size;

      // allocate section
      auto buffer = std::make_unique<uint8_t[]>(len);

      // seek to image offset
      reader_->Seek(hdr->sh_offset);

      // read text section into buffer
      reader_->Read(reinterpret_cast<char*>(buffer.get()), len);

      return buffer;
    }
  }

  len = 0;
  return nullptr;
}

uint16_t ElfReader::section_count() const { return working_hdr_->e_shnum; }

const UPtrElfHeader& ElfReader::header() const { return working_hdr_; }

const UPtrStringTable& ElfReader::string_table() const { return string_table_; }

}  // namespace efx
