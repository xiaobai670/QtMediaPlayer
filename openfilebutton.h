#ifndef OPENFILEBUTTON_H
#define OPENFILEBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>

class OpenFileButton : public QWidget
{
    Q_OBJECT
public:
    explicit OpenFileButton(QWidget *parent = nullptr);

private:
    // 打开文件按钮
    QPushButton *btnOpenFile = nullptr;

    // 布局
    QHBoxLayout *hlayout = nullptr;

    void iniUI();
    void qssUI();

signals:
    sigOpenFile();
};

#endif // OPENFILEBUTTON_H
