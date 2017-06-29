#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <ios>

namespace efx {

#define pure 0

class BinaryReader {
 public:
  virtual void Seek(const std::streampos off) = pure;
  virtual void Skip(const std::streamsize len) = pure;
  virtual std::streampos pos() = pure;

  virtual void Read(char *dst, const size_t len) = pure;

  template <typename T>
  T Read() {
    static_assert(!std::is_fundamental<T>::type
                  && std::is_unsigned<T>::type,
                  "Type must be primitive");

    T buffer{0};
    this->Read(&buffer, sizeof(T));
    return buffer;
  }

  // handle endianness here? (obscurecolin)
};

}  // namespace efx

#endif  // BINARYREADER_H_
