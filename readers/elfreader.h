#ifndef ELFREADER_H
#define ELFREADER_H

#include <ios>
#include <memory>
#include <string>

#include "../formats/elf.h"
#include "filereader.h"
#include "memreader.h"

#include <iostream>

namespace efx {

class ElfReader;
using UPtrElfReader = std::unique_ptr<ElfReader>;

class ElfReader {
 public:
  /* implicitly deletes copy constructor(s), this class is only movable to
   * impose
   * ownership limitation */
  ElfReader(ElfReader&& other);
  ElfReader& operator=(ElfReader&& other);

  static UPtrElfReader CreateFromFile(const std::string& fileLocation);
  static UPtrElfReader CreateFromMemory(const uint8_t* const buffer,
                                        const std::streamsize len);

  uint64_t entry() const;

  /**
   * @return immutable reference to the working header being used by this reader
   * Perhaps improve encapsulation by facading access to this via public methods
   * (obscurecolin)
   */
  const std::unique_ptr<ElfHeader>& header() const;

 private:
  std::unique_ptr<BinaryReader> reader_;
  std::unique_ptr<ElfHeader> working_hdr_;

  // TODO: private constructor for encapsulated dependency injection
  // (obscurecolin)

  ElfReader(std::unique_ptr<BinaryReader> reader);
};

}  // namespace efx

#endif  // ELFREADER_H
