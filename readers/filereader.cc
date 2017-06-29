#include "filereader.h"

namespace efx {

FileReader::FileReader(const std::string &fileLocation)
    : ifs_(fileLocation, (std::ios::binary | std::ios::ate)),
      len_{ifs_.tellg()} {}

// seek skip pos

void FileReader::Seek(const std::streampos off) { ifs_.seekg(off); }

void FileReader::Skip(const std::streamsize len) {
  const std::streampos kCurrent = pos();
  Seek(kCurrent + len);
}

std::streampos FileReader::pos() { return ifs_.tellg(); }

/**
 * Facade for std::ifstream because template methods cannot be virtual
 * @param dst destination
 * @param len length (of type; this method is only used for fundamental types)
 */
void FileReader::Read(char *dst, const size_t len) {
  if (!dst || (len == 0)) return;
  ifs_.read(dst, len);
}

}  // namespace efx
