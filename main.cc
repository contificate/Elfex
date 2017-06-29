#include <QApplication>
#include "elfexwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // perhaps load theme configuration stuff here
  ElfexWindow w;
  w.show();

  auto ret = app.exec();

  // perhaps global singleton cleanup here

  return ret;
}
