#ifndef BOTTOMCONTROLBAR_H
#define BOTTOMCONTROLBAR_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStyle>
#include <QPainter>
#include <QBrush>

class BottomControlBar : public QWidget
{
    Q_OBJECT
public:
    explicit BottomControlBar(QWidget *parent = nullptr);
    void setBackgroundColor(QColor color);
    void setEnableNext(bool enabled);
    void setEnablePrevious(bool enabled);
    void setEnablePlay(bool enabled);

signals:
    void sigPause();
    void sigPlay();
    void sigPrevious();
    void sigNext();
    void sigFullScreen();
    void sigMovePlayPosition(qint64 position);
    void sigMutedChanged(bool muted);
    void sigVolumeChange(float volume);

private slots:
    void emitPlayOrPause(bool isPlay);
    void emitMovePlayPosition(int iPosition);
    void emitVocieMuted(bool muted);
    void emitVolumeChange(int voice);

public slots:
    void setPlay(bool isPlay);
    void setSliderTimeValue(qint64 value);
    void setSLiderTimeMaxnum(qint64 duration);
    void setMuted(bool muted);
    void setSliderVoiceValue(float value);

protected:
    void paintEvent(QPaintEvent *event);

private:
    // QWidget *widParent;
    QPushButton *btnPlay = nullptr;
    QPushButton *btnPrevious = nullptr;
    QPushButton *btnNext = nullptr;
    QPushButton *btnFullScreen = nullptr;
    QPushButton *btnVoice = nullptr;
    QSlider *sliderTime = nullptr;
    QSlider *sliderVoice = nullptr;
    QLabel *labStartTime = nullptr;
    QLabel *labEndTime = nullptr;
    QHBoxLayout *hlayoutBar = nullptr;
    QHBoxLayout *hlayoutButton = nullptr;
    QVBoxLayout *vlayoutControl = nullptr;

    QColor backgroundColor; // 背景颜色

    void iniUI();
    void qssUI();


};

#endif // BOTTOMCONTROLBAR_H
