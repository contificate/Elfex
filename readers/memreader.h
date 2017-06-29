#ifndef MEMREADER_H
#define MEMREADER_H

#include <cstdint>
#include <ios>

#include "binaryreader.h"

namespace efx {

class MemReader : public BinaryReader {
 public:
  MemReader(const uint8_t *const buffer, const std::streamsize len);

  /**
   * Overriden methods from BinaryReader
   * @see BinaryReader
   */
  void Seek(const std::streampos off) override;
  void Skip(const std::streamsize len) override;
  std::streampos pos() override;

  void Read(char *dst, const size_t len) override;

 private:
  const uint8_t *buffer_;
  const std::streamsize len_;
  std::streampos current_off_;
};

}  // namespace efx

#endif  // MEMREADER_H
