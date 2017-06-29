#ifndef ELFREADER_H
#define ELFREADER_H

#include <ios>
#include <memory>
#include <string>

#include "binaryreader.h"

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

 private:
  std::unique_ptr<BinaryReader> reader_;
  // TODO: private constructor for encapsulated dependency injection
  // (obscurecolin)
};

}  // namespace efx

#endif  // ELFREADER_H
