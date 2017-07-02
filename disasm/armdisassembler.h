#ifndef ARMDISASSEMBLER_H
#define ARMDISASSEMBLER_H


#include "disassembler.h"

namespace efx {

class ARMDisassembler : public Disassembler
{
public:
    ARMDisassembler(const std::string &fileLocation);


    // model cleanup?
    ~ARMDisassembler();

private:
    void BuildModel(const cs_insn& instruction) override;
};

} // namespace efx

#endif // ARMDISASSEMBLER_H
