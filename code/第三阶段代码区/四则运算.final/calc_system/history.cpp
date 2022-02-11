#include "history.h"
#include "ui_history.h"
#include <QFile>
#include <QTextStream>
history::history(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::history)
{
    ui->setupUi(this);
    QFile file("历史记录.txt");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream out(&file);
    QString line;
    line=out.readLine();
    while(!line.isNull())
    {
        ui->browser->append(line);
        line=out.readLine();
    }
    file.close();



}

history::~history()
{
    delete ui;
}
