#ifndef CVIDEOWIDGET_H
#define CVIDEOWIDGET_H

#include <QWidget>
#include <QVideoWidget>

class CVideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit CVideoWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event);
signals:
};

#endif // CVIDEOWIDGET_H
