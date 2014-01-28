#include "instrumentwindow.h"
#include "ui_instrumentwindow.h"

InstrumentWindow::InstrumentWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentWindow)
{
    ui->setupUi(this);
}

InstrumentWindow::~InstrumentWindow()
{
    delete ui;
}
