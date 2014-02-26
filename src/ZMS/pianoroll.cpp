#include "pianoroll.h"
#include "ui_pianoroll.h"

PianoRoll::PianoRoll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PianoRoll)
{
    ui->setupUi(this);
}

PianoRoll::~PianoRoll()
{
    delete ui;
}
