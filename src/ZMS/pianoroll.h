#ifndef PIANOROLL_H
#define PIANOROLL_H

#include <QWidget>

namespace Ui {
class PianoRoll;
}

class PianoRoll : public QWidget
{
    Q_OBJECT

public:
    explicit PianoRoll(QWidget *parent = 0);
    ~PianoRoll();

private:
    Ui::PianoRoll *ui;
};

#endif // PIANOROLL_H
