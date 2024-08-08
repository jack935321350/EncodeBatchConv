#include "mylineeditdir.h"

MyLineEditDir::MyLineEditDir(QWidget *parent) : QLineEdit(parent)
{

}

MyLineEditDir::~MyLineEditDir()
{

}

void MyLineEditDir::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    QSettings settings("QtProject", "EncodeConv");
    QString dirName = QFileDialog::getExistingDirectory(this, "Select a Dir.", settings.value("last_dir").toString());
    QDir dir(dirName);
    if(!dirName.isEmpty() && dir.exists())
    {
        dir.cdUp();
        settings.setValue("last_dir", dir.absolutePath());
        settings.sync();
        setText(dirName);
    }
}
