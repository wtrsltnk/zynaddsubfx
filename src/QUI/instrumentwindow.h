#ifndef INSTRUMENTWINDOW_H
#define INSTRUMENTWINDOW_H

#include <QWidget>

namespace Ui {
class InstrumentWindow;
}

class InstrumentWindow : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentWindow(QWidget *parent = 0);
    ~InstrumentWindow();

private:
    Ui::InstrumentWindow *ui;
};

#endif // INSTRUMENTWINDOW_H
