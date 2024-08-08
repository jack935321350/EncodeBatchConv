#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    setWindowTitle("Batch GBK -> UTF-8");
}

MainWidget::~MainWidget()
{
    delete ui;
}

bool MainWidget::GbkCheck(QString srcFileName)
{
    QFile srcFile(srcFileName);

    if (!srcFile.open(QFile::ReadOnly)) {
        return false;
    }

    QByteArray ba = srcFile.readAll();

    srcFile.close();

    QTextCodec::ConverterState cs;
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    tc->toUnicode(ba.constData(), ba.size(), &cs);

    if(cs.invalidChars > 0) {
        return false;
    }

    return true;
}

void MainWidget::Gbk2Utf8(QString srcFileName, QString dstFileName)
{
    QFile srcFile(srcFileName);
    QFile dstFile(dstFileName);

    if (!srcFile.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    if (!dstFile.open(QFile::WriteOnly | QFile::Text)) {
        srcFile.close();
        return;
    }

    QTextStream in(&srcFile);
    in.setCodec("GBK");

    QTextStream out(&dstFile);
    out.setCodec("UTF-8");

    while (!in.atEnd()) {
        out << in.readLine() << Qt::endl;
    }

    srcFile.close();
    dstFile.close();
}

void MainWidget::RecursiveConv(QString srcDirPath, QString dstDirPath)
{
    QDir srcDir(srcDirPath);
    srcDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    QDir dstDir(dstDirPath);

    QFileInfoList srcFileInfoList = srcDir.entryInfoList();
    for(const QFileInfo &srcFileInfo : qAsConst(srcFileInfoList))
    {
        if(srcFileInfo.isDir())
        {
            if(!QFileInfo::exists(dstDir.filePath(srcFileInfo.fileName()))) {
                QDir().mkdir(dstDir.filePath(srcFileInfo.fileName()));
            }
            RecursiveConv(srcFileInfo.absoluteFilePath(), dstDir.absoluteFilePath(srcFileInfo.fileName()));
        }
        else if(srcFileInfo.suffix() == "h" || srcFileInfo.suffix() == "c" || srcFileInfo.suffix() == "cc" ||
                srcFileInfo.suffix() == "cpp" || srcFileInfo.suffix() == "cxx")
        {
            if(GbkCheck(srcFileInfo.absoluteFilePath())) {
                Gbk2Utf8(srcFileInfo.absoluteFilePath(), dstDir.absoluteFilePath(srcFileInfo.fileName()));
            } else {
                QFile::setPermissions(dstDir.absoluteFilePath(srcFileInfo.fileName()), QFile::ReadOther | QFile::WriteOther);
                QFile::remove(dstDir.absoluteFilePath(srcFileInfo.fileName()));
                QFile::copy(srcFileInfo.absoluteFilePath(), dstDir.absoluteFilePath(srcFileInfo.fileName()));
            }
        }
        else
        {
            QFile::setPermissions(dstDir.absoluteFilePath(srcFileInfo.fileName()), QFile::ReadOther | QFile::WriteOther);
            QFile::remove(dstDir.absoluteFilePath(srcFileInfo.fileName()));
            QFile::copy(srcFileInfo.absoluteFilePath(), dstDir.absoluteFilePath(srcFileInfo.fileName()));
        }
    }
}

void MainWidget::on_encodeConv_clicked()
{
    ui->stateLbl->clear();

    RecursiveConv(ui->srcDir->text(), ui->dstDir->text());

    ui->stateLbl->setText(u8"<font color=green>OK...</font>");
}

