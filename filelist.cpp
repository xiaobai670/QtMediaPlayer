#include "filelist.h"

FileList::FileList(QWidget *parent)
    : QWidget{parent, Qt::FramelessWindowHint | Qt::Dialog}
{
    iniUI();
    qssUI();
}

void FileList::addItem(const QString &filepath)
{
    QFileInfo fileInfo(filepath);
    QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
    item->setData(Qt::UserRole, QUrl::fromLocalFile(filepath));
    widFileList->addItem(item);
}

void FileList::setCurrentRow(int index)
{
    widFileList->setCurrentRow(index);
}

QListWidgetItem *FileList::currentItem()
{
    return widFileList->currentItem();
}

int FileList::currentRow()
{
    return widFileList->currentRow();
}

int FileList::count()
{
    return widFileList->count();
}

void FileList::iniUI()
{
    setAttribute(Qt::WA_TranslucentBackground);

    widBack = new QWidget(this);
    btnShow = new QPushButton(this);
    btnAddFile = new QPushButton(this);
    widFileList = new QListWidget(this);
    vlayMiddle = new QVBoxLayout();
    vlayList = new QVBoxLayout(widBack);
    hlayLeft = new QHBoxLayout();
    hlayMain = new QHBoxLayout(this);

    vlayMiddle->addStretch();
    vlayMiddle->addWidget(btnShow);
    vlayMiddle->addStretch();

    hlayLeft->addStretch();
    hlayLeft->addWidget(btnAddFile);

    vlayList->addLayout(hlayLeft);
    vlayList->addWidget(widFileList);
    vlayList->setSpacing(0);
    vlayList->setContentsMargins(0,0,0,0);

    hlayMain->addStretch();
    hlayMain->addLayout(vlayMiddle);
    hlayMain->addWidget(widBack);
    hlayMain->setSpacing(0);
    hlayMain->setContentsMargins(0,0,0,0);

    btnShow->setFixedSize(30, 50);
    btnShow->setIcon(QIcon(":/media/icons/fill_media_list_show.svg"));
    btnAddFile->setFixedSize(20,20);
    btnAddFile->setIcon(QIcon(":/media/icons/fill_media_list_add.svg"));

    widBack->hide();

    connect(btnShow, SIGNAL(clicked()), this, SLOT(setUnfold()));
    connect(btnAddFile, SIGNAL(clicked()), this, SIGNAL(sigAddFiles()));
    connect(widFileList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(emitOpenItem(QModelIndex)));

}

void FileList::qssUI()
{
    widBack->setStyleSheet("background-color: rgba(50,50,50,100);");

    widFileList->setStyleSheet("background-color: rgba(50,50,50,100);"
                               "color: rgba(255, 255, 255, 255);");

    btnShow->setStyleSheet("background-color: rgba(50,50,50,100);");

    btnAddFile->setStyleSheet("background-color: rgba(100,100,100,100);");

}

void FileList::setUnfold()
{
    if (widBack->isVisible())
    {
        widBack->hide();
        btnShow->setIcon(QIcon(":/media/icons/fill_media_list_show.svg"));
    }
    else
    {
        widBack->show();
        btnShow->setIcon(QIcon(":/media/icons/fill_media_list_hide.svg"));
    }

}

void FileList::emitOpenItem(const QModelIndex &index)
{
    Q_UNUSED(index)
    emit sigOpenItem();
}
