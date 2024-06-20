#include "cpushbutton.h"


void CPushButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(255,0,0,255));

    return;
}
