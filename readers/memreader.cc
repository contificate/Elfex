#include "memreader.h"

namespace efx {

MemReader::MemReader(const uint8_t *const buffer, const std::streamsize len)
    : buffer_{buffer}, len_{len} {}

void MemReader::Seek(const std::streampos off) {
  if (off >= len_) return;
  current_off_ = off;
}

void MemReader::Skip(const std::streamsize len) {
  const std::streampos kCurrent = pos();
  Seek(kCurrent + len);
}

std::streampos MemReader::pos() { return current_off_; }

/**
 * Facade for reading previously mapped memory because template methods cannot
 * be virtual
 * @param dst destination
 * @param len length (of type; this method is only used for fundamental types)
 */
void MemReader::Read(char *dst, const size_t len) {
  if (!dst || (len == 0)) return;

   for(size_t i = 0; i < len; i++)
       *(dst + i) = *((buffer_ + current_off_) + i);

   current_off_ += len;
}

}  // namespace efx
