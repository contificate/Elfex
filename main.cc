#include <QApplication>
#include "elfexwindow.h"

#include <functional>
#include <future>
#include <iostream>
#include <thread>

#include "disasm/armdisassembler.h"
#include "disasm/exceptions/disasmexception.h"
#include "readers/elfreader.h"

int main() {
  using namespace efx;

  try {
    UPtrElfReader reader =
        ElfReader::CreateFromFile("/home/dosto/Downloads/tesco");

    uint64_t length = 0;
    if (auto textSection = reader->ReadTextSection(length)) {
      // I want this to work off of working directories
      const char* sectionFile = "/home/dosto/section.text";

      // write to working directory file
      std::ofstream ofs(sectionFile, std::ios::binary);
      ofs.write(reinterpret_cast<char*>(textSection.get()), length);
      ofs.close();  // close so working disassembler can attain access to file

      try {
        // create ARM disassembler
        ARMDisassembler disasm(sectionFile);

        // print instructions
        disasm.OnInstruction([](const cs_insn& instruction) -> bool {
          std::cout << instruction.mnemonic << ' ' << instruction.op_str
                    << '\n';
          return true;
        });

        // start disassembly
        disasm.Disassemble(reader->header()->e_entry);
      } catch (const DisasmException& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
      }
    }

  } catch (const std::runtime_error& e) {
    std::cout << e.what() << '\n';
  }
}
