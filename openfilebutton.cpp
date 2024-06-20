#include "openfilebutton.h"

OpenFileButton::OpenFileButton(QWidget *parent)
    : QWidget{parent, Qt::FramelessWindowHint | Qt::Dialog}
{
    iniUI();
    qssUI();
}

void OpenFileButton::iniUI()
{
    setAttribute(Qt::WA_TranslucentBackground);

    btnOpenFile = new QPushButton("打开文件", this);

    hlayout = new QHBoxLayout(this);
    hlayout->addWidget(btnOpenFile);

    connect(btnOpenFile, SIGNAL(clicked()), this, SIGNAL(sigOpenFile()));
}

void OpenFileButton::qssUI()
{
    btnOpenFile->setStyleSheet("background-color: rgba(0,100,150,0);"
                               "font-size: 20px;"
                               "font-weight:bold;"
                               "color: rgba(0,100,150,255);"
                               "min-width: 60px;"
                               "min-height: 30px;");
}

