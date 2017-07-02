#include <QApplication>
#include "elfexwindow.h"

#include <functional>
#include <future>
#include <iostream>
#include <thread>

#include "disasm/armdisassembler.h"
#include "disasm/exceptions/disasmexception.h"

int main(int argc, char* argv[]) {
  try {
    // create disassembler from .text dump
    efx::ARMDisassembler tester("/home/dosto/arm.text");

    // callback per instruction (model builder called before this is reached)
    tester.OnInstruction([](const cs_insn& other) -> bool {
      std::cout << other.mnemonic << ' ' << other.op_str << '\n';
      return true;
    });

    // launch example asynchronously
    std::future<bool> result = std::async(
        std::launch::async, &efx::Disassembler::Disassemble, &tester, (1 << 4));

    tester.Disassemble(1 << 4);
  } catch (const efx::DisasmException& kErr) {
    std::cerr << kErr.what() << '\n';
  }

  /*
 // A few tests
 using namespace efx;
 try {
   UPtrElfReader reader = ElfReader::CreateFromFile("/home/dosto/disasm");
   const std::unique_ptr<ElfHeader>& kHeader = reader->header();

   std::cout << "Entry point: " << std::hex << kHeader->e_entry << '\n';

 } catch (const std::runtime_error& kError) {
   std::cerr << kError.what() << '\n';
 }

 QApplication app(argc, argv);

 // perhaps load theme configuration stuff here
 ElfexWindow w;
 w.show();

 auto ret = app.exec();

 // perhaps global singleton cleanup here
 return ret;
         */
}
