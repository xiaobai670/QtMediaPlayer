#include "cvideowidget.h"

CVideoWidget::CVideoWidget(QWidget *parent)
    : QVideoWidget{parent}
{

}

void CVideoWidget::paintEvent(QPaintEvent *event)
{
    qDebug("video paint...");
    QVideoWidget::paintEvent(event);
}
