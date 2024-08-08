#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QDebug>
#include <QWidget>
#include <QFileInfo>
#include <QTextCodec>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWidget;
}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    bool GbkCheck(QString srcFileName);
    void Gbk2Utf8(QString srcFileName, QString dstFileName);
    void RecursiveConv(QString srcDirPath, QString dstDirPath);

private slots:
    void on_encodeConv_clicked();

};
#endif // MAINWIDGET_H
