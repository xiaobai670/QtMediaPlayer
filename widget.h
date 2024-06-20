#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsVideoItem>
#include <QPushButton>
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QGraphicsRectItem>
#include <QVideoWidget>
#include <QLabel>
#include <QStyle>
#include <QResizeEvent>
#include <QVariant>
#include <QTimer>
#include "bottomcontrolbar.h"
#include "openfilebutton.h"
#include "filelist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    bool eventFilter(QObject *watched, QEvent *event);

protected:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);

private slots:
    void doFullScreen();
    void doOpenFiles();
    void doPlayItem();
    void doPlayPrevious();
    void doPlayNext();
    void doCheckButton(QUrl url);
    void doHideBar();
    void doPlayChanged(QMediaPlayer::PlaybackState newState);

private:
    Ui::Widget *ui;

    QMediaPlayer *player = nullptr;
    QAudioOutput *audioOutpt = nullptr;
    QVideoWidget *videoWidget = nullptr;
    QVBoxLayout *vlayMain = nullptr;

    BottomControlBar *bottomControlBar = nullptr;
    OpenFileButton *openFileButton = nullptr;
    FileList *fileList = nullptr;
    QLabel *labImage = nullptr;

    QTimer *timer = nullptr;
    QWidget *mouseTrack = nullptr;
    QVBoxLayout *vlayMouse = nullptr;

};
#endif // WIDGET_H
