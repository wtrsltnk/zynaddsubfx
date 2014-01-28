#include "instrumentcontainer.h"
#include "ui_instrumentcontainer.h"

InstrumentContainer::InstrumentContainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstrumentContainer)
{
    ui->setupUi(this);
}

InstrumentContainer::~InstrumentContainer()
{
    delete ui;
}
