#ifndef INSTRUMENTCONTAINER_H
#define INSTRUMENTCONTAINER_H

#include <QWidget>

namespace Ui {
class InstrumentContainer;
}

class InstrumentContainer : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentContainer(QWidget *parent = 0);
    ~InstrumentContainer();

private:
    Ui::InstrumentContainer *ui;
};

#endif // INSTRUMENTCONTAINER_H
