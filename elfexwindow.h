#ifndef ELFEXWINDOW_H
#define ELFEXWINDOW_H

#include <QMainWindow>

namespace Ui {
class ElfexWindow;
}

class ElfexWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ElfexWindow(QWidget *parent = nullptr);



    ~ElfexWindow();

private:
    Ui::ElfexWindow *ui;
};

#endif // ELFEXWINDOW_H
