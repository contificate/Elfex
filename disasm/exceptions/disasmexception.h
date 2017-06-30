#ifndef DISASMEXCEPTION_H
#define DISASMEXCEPTION_H

#include <exception>

namespace efx {

/**
 * @brief The DisasmException class
 * We can use this for all futures and promises during disassembly
 * because you can set exceptions for those.
 */

class DisasmException : public std::exception {
 public:
  DisasmException(const char* const message) : msg_{message} {}

  const char* what() const noexcept { return msg_; }

 private:
  const char* msg_;
};

}  // namespace efx

#endif  // DISASMEXCEPTION_H
