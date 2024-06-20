#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QBrush>

class CPushButton: public QPushButton
{
public:
    using QPushButton::QPushButton;
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // CPUSHBUTTON_H
