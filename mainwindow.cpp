#include "mainwindow.h"
#include "loggingcategories.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->hide();
    ui->menuRec->setEnabled(false);
    connect(this,&MainWindow::sigShowEvent,this,&MainWindow::slotShowEvent,Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotShowEvent()
{
//    qInfo(logInfo()) << "Окно отрисовали";
    int connID;
    setModelConnect();
    connID=selectCentralDB();
    if(connCentralDB(connID)){
        ui->mainToolBar->show();
        ui->menuRec->setEnabled(true);
    }
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    emit sigShowEvent();
}

void MainWindow::setModelConnect()
{
    QSqlDatabase dbopt = QSqlDatabase::database("options");
    modelConnect = new QSqlTableModel(this,dbopt);
    modelConnect->setTable("connections");
    modelConnect->select();
}

int MainWindow::selectCentralDB()
{
    int curIDConnections;
    switch (modelConnect->rowCount()) {
    case 1:
        curIDConnections=0;
        break;
    default:
        break;
    }
    return curIDConnections;
}

bool MainWindow::connCentralDB(int connID)
{
    QString strStatus;
    bool result;
    QSqlDatabase dbcentr = QSqlDatabase::addDatabase("QIBASE","central");

    dbcentr.setHostName(modelConnect->data(modelConnect->index(connID,2)).toString());
    dbcentr.setDatabaseName(modelConnect->data(modelConnect->index(connID,3)).toString());
    dbcentr.setUserName(modelConnect->data(modelConnect->index(connID,4)).toString());
    dbcentr.setPassword(modelConnect->data(modelConnect->index(connID,5)).toString());
//    dbcentr.setPassword("12345678");

    if(!dbcentr.open()) {
        qCritical(logCritical()) << "Не возможно подключится к центральной базе данных" << dbcentr.lastError().text();
        QMessageBox::critical(0,"Ошибка подключения", QString("Не возможно открыть базу данных!\n%1\nПроверьте настройку подключения.")
                .arg(dbcentr.lastError().text()),
                QMessageBox::Ok);
        strStatus = "Ошибка подключения к центральной базе!";
        result =false;

    } else {
        strStatus = "Подключено к базе данных: "+modelConnect->data(modelConnect->index(connID,1)).toString()+" "
                +modelConnect->data(modelConnect->index(connID,2)).toString()+":"
                +modelConnect->data(modelConnect->index(connID,3)).toString();
        result=true;
    }
    ui->statusBar->showMessage(strStatus);
    return result;

}

void MainWindow::on_action_triggered()
{

}
