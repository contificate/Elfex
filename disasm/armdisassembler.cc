#include "armdisassembler.h"

namespace efx {

ARMDisassembler::ARMDisassembler(const std::string& fileLocation)
    : Disassembler(fileLocation, CS_ARCH_ARM, CS_MODE_ARM) {}

/**
 * @brief ARMDisassembler::BuildModel
 * @param instruction instruction being analyzed for the model
 */
void ARMDisassembler::BuildModel(const cs_insn& instruction) {}

ARMDisassembler::~ARMDisassembler() {}

}  // namespace efx
