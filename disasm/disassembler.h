#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <capstone/capstone.h>
#include <cstdint>
#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <iostream>

#include "exceptions/disasmexception.h"

namespace efx {

#define LOAD_FLAGS (std::ios::binary | std::ios::ate)

using InstrCallback = std::function<bool(const cs_insn&)>;

#define pure 0

class Disassembler {
 public:
  Disassembler(const std::string& textSection, const cs_arch arch,
               const cs_mode mode);

  void OnInstruction(const InstrCallback callback);
  bool Disassemble(const uint64_t address);

  virtual ~Disassembler();

 private:
  bool LoadFile(const std::string& file);
  virtual void BuildModel(const cs_insn& instruction) = pure;

  std::function<bool(const cs_insn&)> instr_callback_;
  std::unique_ptr<uint8_t[]> buffer_;
  std::streamsize len_;

 protected:
  csh handle_;
  const cs_arch arch_;
  const cs_mode mode_;
};

}  // namespace efx

#endif  // DISASSEMBLER_H
