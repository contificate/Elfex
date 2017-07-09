#include "disassembler.h"

namespace efx {

// to be called by children
Disassembler::Disassembler(const std::string &textSection, const cs_arch arch,
                           const cs_mode mode)
    : instr_callback_{nullptr},
      buffer_{nullptr},
      len_{0},
      arch_{arch},
      mode_{mode} {
  // attain capstone handle
  if (cs_open(arch, mode, &handle_) != CS_ERR_OK)
    throw DisasmException("Failed to attain capstone handle!");

  // map section into memory
  if (!LoadFile(textSection))
    throw DisasmException("Failed to map section for disassembly!");
}

void Disassembler::OnInstruction(const InstrCallback &callback) {
  instr_callback_ = callback;
}

void Disassembler::OnInstruction(InstrCallback &&callback)
{
    instr_callback_ = std::move(callback);
}

/**
 * Map project's .text file into memory
 * @param file path to file (std::path anyone?)
 * @return whether file got loaded correctly
 */
bool Disassembler::LoadFile(const std::string &file) {
  if (auto ifs = std::ifstream(file, LOAD_FLAGS)) {
    len_ = ifs.tellg();

    // allocate buffer for section
    if (!(buffer_ = std::make_unique<uint8_t[]>(len_))) return false;

    // read section into buffer (should we overload for mmap?)
    ifs.seekg(0, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(buffer_.get()), len_);
    // return !!ifs;
    return ifs.is_open();
  }

  return false;
}

/**
 * @brief Disassembler::Disassemble
 * @param address start address of disassembly (hdr->e_entry)
 * @return overall successfulness of disassembler based off of model creation
 * (?)
 */
bool Disassembler::Disassemble(const uint64_t address) {
  if (!instr_callback_ || !buffer_ || handle_ == CS_ERR_HANDLE) return false;

  // allocate buffer per arch (e.g. 15 bytes max for x86, 4 for ARM, etc.)
  cs_insn *instruction = cs_malloc(handle_);

  uint8_t *code = reinterpret_cast<uint8_t *>(buffer_.get());
  size_t len = static_cast<size_t>(len_);

  while (cs_disasm_iter(handle_, const_cast<const uint8_t **>(&code), &len,
                        const_cast<uint64_t *>(&address), instruction)) {
    this->BuildModel(*instruction);

    // pass to callback context
    instr_callback_(*instruction);
  }

  // free single instruction buffer
  cs_free(instruction, 1);

  // TODO: check model status (obscurecolin)
  return true;
}

// free handle
Disassembler::~Disassembler() { cs_close(&handle_); }

}  // namespace efx
