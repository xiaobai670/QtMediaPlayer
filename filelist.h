#ifndef FILELIST_H
#define FILELIST_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QFileInfo>
#include <QUrl>

class FileList : public QWidget
{
    Q_OBJECT
public:
    explicit FileList(QWidget *parent = nullptr);

    void addItem(const QString &filepath);
    void setCurrentRow(int index);
    QListWidgetItem *currentItem();
    int currentRow();
    int count();

private:
    // 收起、展开按钮
    QPushButton *btnShow = nullptr;

    // 添加文件按钮
    QPushButton *btnAddFile = nullptr;

    // ListWidget
    QListWidget *widFileList = nullptr;

    // 按钮居中布局
    QVBoxLayout *vlayMiddle = nullptr;

    // 添加按钮居左布局
    QHBoxLayout *hlayLeft = nullptr;

    // 按钮ListWidget上下布局
    QVBoxLayout *vlayList = nullptr;

    // 背景界面
    QWidget *widBack = nullptr;

    // 主布局
    QHBoxLayout *hlayMain = nullptr;

    void iniUI();
    void qssUI();

signals:
    void sigAddFiles();
    void sigOpenItem();

private slots:
    void setUnfold();
    void emitOpenItem(const QModelIndex &index);
};

#endif // FILELIST_H
