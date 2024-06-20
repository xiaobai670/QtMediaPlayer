#include "bottomcontrolbar.h"

BottomControlBar::BottomControlBar(QWidget *parent)
    : QWidget(parent, Qt::Dialog | Qt::FramelessWindowHint)
    , backgroundColor(QColor(100, 100, 100, 100)) // 默认背景颜色
{
    iniUI(); // 初始化组件
    qssUI(); // 使用qss美化组件
}

void BottomControlBar::setBackgroundColor(QColor color)
{
    backgroundColor = color;
}

void BottomControlBar::setEnableNext(bool enabled)
{
    btnNext->setEnabled(enabled);
}

void BottomControlBar::setEnablePrevious(bool enabled)
{
    btnPrevious->setEnabled(enabled);
}

void BottomControlBar::setEnablePlay(bool enabled)
{
    btnPlay->setEnabled(enabled);
}

void BottomControlBar::emitPlayOrPause(bool isPlay)
{
    if (isPlay)
    {
        emit sigPlay();
        btnPlay->setIcon(QIcon(":/media/icons/fill_media_pause.svg"));
    }
    else
    {
        emit sigPause();
        btnPlay->setIcon(QIcon(":/media/icons/fill_media_play.svg"));
    }
}

void BottomControlBar::emitMovePlayPosition(int iPosition)
{
    qint64 position = iPosition * 1000;
    emit sigMovePlayPosition(position);

    // 刷新显示时间
    // 转换成时分秒
    int mins = iPosition / 60;
    int hours = mins / 60;
    iPosition = iPosition % 60;
    mins = mins % 60;

    // 设置显示时间
    QString times = QString::asprintf("%02d:%02d:%02d", hours, mins, iPosition);
    labStartTime->setText(times);

}

void BottomControlBar::emitVocieMuted(bool muted)
{
    emit sigMutedChanged(muted);

    if (muted)
    {

        btnVoice->setIcon(QIcon(":/media/icons/fill_media_vol_mute.svg"));
    }
    else
    {
        btnVoice->setIcon(QIcon(":/media/icons/fill_media_vol_default.svg"));
    }
}

void BottomControlBar::emitVolumeChange(int voice)
{
    float volume = (static_cast<float>(voice)) / 100.0; // 必须先将整型转换成浮点型再进行计算，或者除数必须为浮点数
    emit sigVolumeChange(volume);
}

void BottomControlBar::setPlay(bool isPlay)
{
    btnPlay->setChecked(isPlay);
    if (isPlay)
    {
        btnPlay->setIcon(QIcon(":/media/icons/fill_media_pause.svg"));
    }
    else
    {
        btnPlay->setIcon(QIcon(":/media/icons/fill_media_play.svg"));
    }
}

void BottomControlBar::setSliderTimeValue(qint64 value)
{
    // 防止使用滑动条修改播放位置时引起该槽函数启动造成递归调用
    if (sliderTime->isSliderDown())
        return;
    int secs = value / 1000;
    if (secs != sliderTime->value())
        sliderTime->setSliderPosition(secs);

    // 转换成时分秒
    int mins = secs / 60;
    int hours = mins / 60;
    secs = secs % 60;
    mins = mins % 60;

    // 设置显示时间
    QString times = QString::asprintf("%02d:%02d:%02d", hours, mins, secs);
    labStartTime->setText(times);
}

void BottomControlBar::setSLiderTimeMaxnum(qint64 duration)
{
    int secs = duration / 1000;
    sliderTime->setMaximum(secs);

    // 转换成时分秒
    int mins = secs / 60;
    int hours = mins / 60;
    secs = secs % 60;
    mins = mins % 60;

    // 设置显示时间
    QString times = QString::asprintf("%02d:%02d:%02d", hours, mins, secs);
    labEndTime->setText(times);
}

void BottomControlBar::setMuted(bool muted)
{
    btnVoice->setChecked(muted);

    if (muted)
    {
        btnPlay->setIcon(QIcon(":/media/icons/fill_media_vol_mute.svg"));
    }
    else
    {
        btnPlay->setIcon(QIcon(":/media/icons/fill_media_vol_default.svg"));
    }
}

void BottomControlBar::setSliderVoiceValue(float value)
{
    // 防止使用滑动条修改播放位置时引起该槽函数启动造成递归调用
    if (sliderVoice->isSliderDown())
        return;

    int voiceValue = value * 100;

    if (voiceValue != sliderVoice->value())
        sliderVoice->setValue(voiceValue);
}

void BottomControlBar::paintEvent(QPaintEvent *event)
{
    //绘制背景
    if (backgroundColor.isValid())
    {
        QPainter painter(this);
        QBrush brush(backgroundColor);
        painter.setBrush(brush);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0,0, width(), height());
    }

    QWidget::paintEvent(event);
}

void BottomControlBar::iniUI()
{
    setAttribute(Qt::WA_TranslucentBackground); // 将窗口设置为半透明，窗口上的组件依然是不透明
    setAttribute(Qt::WA_AlwaysStackOnTop);
    update();

    hlayoutBar = new QHBoxLayout();
    labStartTime = new QLabel("00:00:00", this);
    labEndTime = new QLabel("00:00:00", this);
    sliderTime = new QSlider(Qt::Horizontal, this);
    btnFullScreen = new QPushButton(this);
    hlayoutBar->addWidget(labStartTime);
    hlayoutBar->addWidget(sliderTime);
    hlayoutBar->addWidget(labEndTime);
    hlayoutBar->addWidget(btnFullScreen);

    hlayoutButton = new QHBoxLayout();
    btnPlay = new QPushButton(this);
    btnPlay->setCheckable(true);
    btnPlay->setChecked(false);
    btnPrevious = new QPushButton(this);
    btnNext = new QPushButton(this);
    btnVoice = new QPushButton(this);
    btnVoice->setCheckable(true);
    btnVoice->setChecked(false);
    sliderVoice = new QSlider(Qt::Horizontal, this);
    sliderVoice->setMaximum(100);  //将声音最大值改为100
    hlayoutButton->addStretch();
    hlayoutButton->addWidget(btnPrevious);
    hlayoutButton->addWidget(btnPlay);
    hlayoutButton->addWidget(btnNext);
    hlayoutButton->addStretch();
    hlayoutButton->addWidget(btnVoice);
    hlayoutButton->addWidget(sliderVoice);
    hlayoutButton->addStretch();


    vlayoutControl = new QVBoxLayout(this);
    vlayoutControl->addLayout(hlayoutBar);
    vlayoutControl->addLayout(hlayoutButton);

    // 固定控制栏高度
    setFixedHeight(80);

    // 设置图标
    btnPlay->setIcon(QIcon(":/media/icons/fill_media_play.svg"));
    btnPrevious->setIcon(QIcon(":/media/icons/fill_media_previous.svg"));
    btnNext->setIcon(QIcon(":/media/icons/fill_media_next.svg"));
    btnVoice->setIcon(QIcon(":/media/icons/fill_media_vol_default.svg"));
    btnFullScreen->setIcon(QIcon(":/media/icons/fill_media_airplay.svg"));

    // 关联信号与槽
    connect(btnPlay, SIGNAL(clicked(bool)), this, SLOT(emitPlayOrPause(bool))); // 播放暂停信号
    connect(btnPrevious, SIGNAL(clicked()), this, SIGNAL(sigPrevious())); // 上一首信号
    connect(btnNext, SIGNAL(clicked()), this, SIGNAL(sigNext())); // 下一首信号
    connect(btnFullScreen, SIGNAL(clicked()), this, SIGNAL(sigFullScreen())); // 全屏信号
    connect(sliderTime, SIGNAL(sliderMoved(int)), this, SLOT(emitMovePlayPosition(int))); // 时间进度条移动信号
    connect(btnVoice, SIGNAL(clicked(bool)), this, SLOT(emitVocieMuted(bool))); // 静音信号
    connect(sliderVoice, SIGNAL(sliderMoved(int)), this, SLOT(emitVolumeChange(int)));
}

void BottomControlBar::qssUI()
{

    btnPlay->setStyleSheet("background-color: rgba(0,100,150,200);"
                           "border-radius: 15px;"
                           "min-width: 30px;"
                           "min-height: 30px;");
    btnPrevious->setStyleSheet("background-color: rgba(0,100,150,0);"
                           "border-radius: 10px;"
                           "min-width: 20px;"
                           "min-height: 20px;");
    btnNext->setStyleSheet("background-color: rgba(0,100,150,0);"
                           "border-radius: 10px;"
                           "min-width: 20px;"
                           "min-height: 20px;");
    btnVoice->setStyleSheet("background-color: rgba(0,100,150,0);"
                           "border-radius: 5px;"
                           "min-width: 20px;"
                           "min-height: 20px;");
    btnFullScreen->setStyleSheet("background-color: rgba(0,100,150,0);"
                            "min-width: 20px;"
                           "min-height: 20px;");
    labStartTime->setStyleSheet("background-color: rgba(0,100,150,0);"
                                "color: rgba(0, 100, 150, 255);"
                                "min-width: 30px;"
                                "min-height: 30px;");
    labEndTime->setStyleSheet("background-color: rgba(0,100,150,0);"
                              "color: rgba(0, 100, 150, 255);"
                              "min-width: 30px;"
                              "min-height: 30px;");
    sliderTime->setStyleSheet("background-color: rgba(0,100,150,0);");
}
