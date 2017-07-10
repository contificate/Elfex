#include <QApplication>
#include "elfexwindow.h"

#include <functional>
#include <future>
#include <iostream>
#include <thread>

#include "disasm/exceptions/disasmexception.h"
#include "readers/elfreader.h"

int main() {
  using namespace efx;

  try {
    UPtrElfReader reader = ElfReader::CreateFromFile("/home/dosto/disasm");
    std::cout << reader->string_table().get() + 2
              << '\n';  // +2 = skip null terminators at start of table and at
                        // start of first entry

  } catch (const std::runtime_error& e) {
    std::cout << e.what() << '\n';
  }
}
