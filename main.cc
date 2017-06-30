#include <QApplication>
#include "elfexwindow.h"

#include <iostream>

#include "readers/elfreader.h"

int main(int argc, char* argv[]) {

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
}
