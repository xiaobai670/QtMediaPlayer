#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)

{
    ui->setupUi(this);
    setMinimumSize(400, 300);
    setWindowTitle("简易播放器");

    audioOutpt = new QAudioOutput(this);
    videoWidget = new QVideoWidget(this);
    mouseTrack = new QWidget(this);
    vlayMouse = new QVBoxLayout(videoWidget);
    vlayMouse->setSpacing(0);
    vlayMouse->setContentsMargins(0,0,0,0);
    vlayMouse->addWidget(mouseTrack);
    mouseTrack->installEventFilter(this);
    mouseTrack->setMouseTracking(true);

    vlayMain = new QVBoxLayout(this);
    vlayMain->addWidget(videoWidget);
    vlayMain->setSpacing(0);
    vlayMain->setContentsMargins(0,0,0,0);


    player = new QMediaPlayer(this);

    bottomControlBar = new BottomControlBar(this);
    if (audioOutpt->isMuted())
        bottomControlBar->setMuted(true);
    bottomControlBar->setSliderVoiceValue(audioOutpt->volume());
    bottomControlBar->show();
    bottomControlBar->installEventFilter(this);
    bottomControlBar->setMouseTracking(true);
    bottomControlBar->setEnablePlay(false);
    bottomControlBar->setEnableNext(false);
    bottomControlBar->setEnablePrevious(false);

    openFileButton = new OpenFileButton(this);
    openFileButton->show();
    openFileButton->installEventFilter(this);
    openFileButton->setMouseTracking(true);

    fileList = new FileList(this);
    fileList->show();
    fileList->installEventFilter(this);
    fileList->setMouseTracking(true);

    timer = new QTimer(this);

    player->setAudioOutput(audioOutpt);
    player->setVideoOutput(videoWidget);

    // 连接信号与槽
    connect(bottomControlBar, SIGNAL(sigPlay()), player, SLOT(play()));
    connect(bottomControlBar, SIGNAL(sigPause()), player, SLOT(pause()));
    connect(player, SIGNAL(durationChanged(qint64)), bottomControlBar, SLOT(setSLiderTimeMaxnum(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), bottomControlBar, SLOT(setSliderTimeValue(qint64)));
    connect(bottomControlBar, SIGNAL(sigMovePlayPosition(qint64)), player, SLOT(setPosition(qint64)));
    connect(bottomControlBar, SIGNAL(sigFullScreen()), this, SLOT(doFullScreen()));
    connect(bottomControlBar, SIGNAL(sigMutedChanged(bool)), audioOutpt, SLOT(setMuted(bool)));
    connect(bottomControlBar, SIGNAL(sigVolumeChange(float)), audioOutpt, SLOT(setVolume(float)));
    connect(bottomControlBar, SIGNAL(sigPrevious()), this, SLOT(doPlayPrevious()));
    connect(bottomControlBar, SIGNAL(sigNext()), this, SLOT(doPlayNext()));
    connect(player, SIGNAL(sourceChanged(QUrl)), this, SLOT(doCheckButton(QUrl)));
    connect(player, SIGNAL(playbackStateChanged(QMediaPlayer::PlaybackState)), this, SLOT(doPlayChanged(QMediaPlayer::PlaybackState)));

    connect(openFileButton, SIGNAL(sigOpenFile()), this, SLOT(doOpenFiles()));

    connect(fileList, SIGNAL(sigAddFiles()), this, SLOT(doOpenFiles()));
    connect(fileList, SIGNAL(sigOpenItem()), this, SLOT(doPlayItem()));

    connect(timer, SIGNAL(timeout()), this, SLOT(doHideBar()));


}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        bottomControlBar->show();
        fileList->show();
        // if (!player->playbackState())
        // {
        //     openFileButton->show();
        // }

        timer->stop();
        timer->setInterval(5000);
        timer->start();

        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void Widget::showEvent(QShowEvent *event)
{
    int subHeight = bottomControlBar->height();
    // 在显示事件中，窗口的几何形状才开始更新
    // 更改控制栏的几何形状，使控制栏显示在底部
    bottomControlBar->setGeometry(x(), geometry().bottom() - subHeight, width(), subHeight);

    // 使打开文件按钮显示在中间
    int btnFileWidth = openFileButton->width();
    int btnFileHeight = openFileButton->height();
    openFileButton->move(x() + width()/2 - btnFileWidth/2, y() + height()/2 - btnFileHeight/2);

    fileList->setGeometry(x() + width() - 300, geometry().top(), 300, height() - subHeight);

    QWidget::showEvent(event);

}

void Widget::resizeEvent(QResizeEvent *event)
{

    int subHeight = bottomControlBar->height();
    bottomControlBar->setGeometry(x(), geometry().bottom() - subHeight, event->size().width(), subHeight);

    int btnFileWidth = openFileButton->width();
    int btnFileHeight = openFileButton->height();
    openFileButton->move(x() + event->size().width()/2 - btnFileWidth/2, y() + event->size().height()/2 - btnFileHeight/2);

    fileList->setGeometry(x() + event->size().width() - 300, geometry().top(), 300, event->size().height() - subHeight);

    QWidget::resizeEvent(event);
}

void Widget::moveEvent(QMoveEvent *event)
{
    if (!isVisible())
    {
        return;
    }
    int subHeight = bottomControlBar->height();
    bottomControlBar->setGeometry(event->pos().x(), geometry().bottom() - subHeight, bottomControlBar->width(), subHeight);

    int btnFileWidth = openFileButton->width();
    int btnFileHeight = openFileButton->height();
    openFileButton->move(x() + width()/2 - btnFileWidth/2, y() + height()/2 - btnFileHeight/2);

    fileList->setGeometry(x() + width() - 300, geometry().top(), 300, height() - subHeight);

    QWidget::moveEvent(event);
}

void Widget::doFullScreen()
{
    if (isFullScreen())
    {
        showNormal();
    }
    else
    {
        showFullScreen();

        // 刷新控制栏和视频输出栏
        int subHeight = bottomControlBar->height();
        bottomControlBar->setGeometry(0, geometry().bottom() - subHeight, width(), subHeight);
    }

}

void Widget::doOpenFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "请选择视频文件", QDir::currentPath(), "视频文件(*.mp4);;所有文件(*.*)");

    if (!files.count())
    {
        return;
    }

    // 将列表添加到视频文件列表中
    for (qsizetype i = 0; i != files.count(); ++i)
    {
        fileList->addItem(files.at(i));
    }

    // 获取播放状态
    if (player->playbackState() == QMediaPlayer::StoppedState)
    {
        fileList->setCurrentRow(0);
        QListWidgetItem *item = fileList->currentItem();
        player->setSource(item->data(Qt::UserRole).toUrl());
        player->play();
        bottomControlBar->setPlay(true);
    }
}

void Widget::doPlayItem()
{
    QListWidgetItem *item = fileList->currentItem();
    player->setSource(item->data(Qt::UserRole).toUrl());
    player->play();
    bottomControlBar->setPlay(true);
}

void Widget::doPlayPrevious()
{
    int curRow = fileList->currentRow();
    fileList->setCurrentRow(curRow - 1);
    doPlayItem();
}

void Widget::doPlayNext()
{
    int curRow = fileList->currentRow();
    fileList->setCurrentRow(curRow + 1);
    doPlayItem();
}

void Widget::doCheckButton(QUrl url)
{
    Q_UNUSED(url)
    bottomControlBar->setEnablePlay(true);
    int curRow = fileList->currentRow();
    int endRow = fileList->count() - 1;

    if (curRow == 0)
    {
        bottomControlBar->setEnablePrevious(false);
        if (endRow == 0)
            bottomControlBar->setEnableNext(false);
        else
            bottomControlBar->setEnableNext(true);
    }
    else if (curRow == endRow)
    {
        bottomControlBar->setEnablePrevious(true);
        bottomControlBar->setEnableNext(false);
    }
    else
    {
        bottomControlBar->setEnablePrevious(true);
        bottomControlBar->setEnableNext(true);
    }
}

void Widget::doHideBar()
{
    if (player->playbackState())
    {
        bottomControlBar->hide();
        openFileButton->hide();
        fileList->hide();
    }
}

void Widget::doPlayChanged(QMediaPlayer::PlaybackState newState)
{
    if (newState)
    {
        openFileButton->hide();
    }
    else
    {
        openFileButton->show();
    }
}
