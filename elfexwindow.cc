#include "elfexwindow.h"
#include "ui_elfexwindow.h"

ElfexWindow::ElfexWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ElfexWindow) {

  ui->setupUi(this);
}

ElfexWindow::~ElfexWindow() { delete ui; }
