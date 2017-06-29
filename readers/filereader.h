#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <string>

#include "binaryreader.h"

namespace efx {

class FileReader : public BinaryReader {
 public:
  explicit FileReader(const std::string &fileLocation);

  /**
   * Overriden methods from BinaryReader
   * @see BinaryReader
   */
  void Seek(const std::streampos off) override;
  void Skip(const std::streamsize len) override;
  std::streampos pos() override;

  void Read(char *dst, const size_t len) override;

 private:
  std::ifstream ifs_;
  const std::streamsize len_;
};

}  // namespace efx

#endif  // FILEREADER_H
