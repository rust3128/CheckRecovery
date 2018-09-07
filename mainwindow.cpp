#include "mainwindow.h"
#include "loggingcategories.h"
#include "terminallistdialog.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpSocket = new QTcpSocket(this);
    ui->mainToolBar->hide();
    ui->groupBoxCheckType->hide();
    createStatusBar();
    ui->groupBoxTerminal->hide();
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

        setModelTerminals();
        ui->groupBoxTerminal->show();
        ui->labelTerminlInfo->setText("Терминал не указан!");
        ui->labelOnLineStatus->hide();
        ui->line->hide();
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
//    dbcentr.setHostName("192.168.200.22");
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
        statusLabel->setStyleSheet("color: rgb(255, 0, 0)"
                                   "border-style: solid;"
                                   "border-width: 1px;"
                                   "border-color: black;");
        result =false;

    } else {
        strStatus = "Подключено к базе данных: "+modelConnect->data(modelConnect->index(connID,1)).toString()+" "
                +modelConnect->data(modelConnect->index(connID,2)).toString()+":"
                +modelConnect->data(modelConnect->index(connID,3)).toString();
        statusLabel->setStyleSheet("color: rgb(0, 85, 0);"
                                   "border-style: solid;"
                                   "border-width: 1px;"
                                   "border-color: black;");
        result=true;
    }
    statusLabel->setText(strStatus);
    return result;

}

void MainWindow::createStatusBar()
{
    statusLabel = new QLabel(this);
    ui->statusBar->addPermanentWidget(statusLabel);
}

bool MainWindow::validateAZS(int idx)
{
    ui->labelOnLineStatus->show();

    tcpSocket->abort();
    tcpSocket->connectToHost(modelTerminals->data(modelTerminals->index(idx,2)).toString(),3050);
    if(tcpSocket->waitForConnected(30000)){
        qInfo(logInfo()) << "Проверка доступности. Сервер: " << modelTerminals->data(modelTerminals->index(idx,2)).toString() <<  "FireBird доступен.";
        ui->labelOnLineStatus->setStyleSheet("color: rgb(0, 170, 0);font: 75 14pt 'Noto Sans'");
        ui->labelOnLineStatus->setText("АЗС на связи!");
        ui->line->show();
        return true;
    } else {
        qInfo(logInfo()) << "Проверка доступности. Сервер: " << modelTerminals->data(modelTerminals->index(idx,2)).toString() <<  "FireBird НЕ доступен.";
        ui->labelOnLineStatus->setStyleSheet("color: red;font: 75 14pt 'Noto Sans'");
        ui->labelOnLineStatus->setText("Отсутствует связь с АЗС!");
        ui->line->show();
        return false;
    }

}

void MainWindow::initLostCheckFuels()
{
    lostCheck.insert("TERMINAL_ID",0);
    lostCheck.insert("SHIFT_ID",0);
    lostCheck.insert("DISPENSER_ID",-1);
    lostCheck.insert("TRK_ID",0);
    lostCheck.insert("TANK_ID",0);
    lostCheck.insert("FUEL_ID",-1);
    lostCheck.insert("GIVE",0);
    lostCheck.insert("ORDERED",0);
    lostCheck.insert("SUMMA",0);
    lostCheck.insert("CASH",0);
    lostCheck.insert("DISCOUNTSUMMA",0);
    lostCheck.insert("PAYTYPE_ID",-1);
    lostCheck.insert("NUM_CHECK",0);
    lostCheck.insert("NUM_CHECK_RETURN",0);
    lostCheck.insert("TRANSACTION_ID",0);
    lostCheck.insert("SEC",0);
    lostCheck.insert("ISLAST","T");
    lostCheck.insert("INFO_CODE",-1);
    lostCheck.insert("INFO_TEXT","");
    lostCheck.insert("POS_ID",1);
    lostCheck.insert("ZNUMBER",0);
    lostCheck.insert("OPERATOR_ID",-1);
    lostCheck.insert("SALEORDER_ID","GEN_ID(GEN_SALEORDERS, 1)");
    lostCheck.insert("PRICE",0);
    lostCheck.insert("ISBEFOREPAY","F");
    lostCheck.insert("POSTRANSACTION_ID",0);
    lostCheck.insert("POSTRNRETURN_ID",0);
    lostCheck.insert("SHARE_ID",0);
    lostCheck.insert("MPOSCHECK_ID",":CHECK_ID");
    lostCheck.insert("PAYTYPE_ID2",1);
    lostCheck.insert("SUMMA2",0.00);
    lostCheck.insert("DISCOUNTSUMMA2",0.00);
    lostCheck.insert("DAT",0);
    lostCheck.insert("GOV_NUMBER","");
    lostCheck.insert("BONUSCARD","");
}

void MainWindow::setModelTerminals()
{
    QSqlDatabase dbcenter = QSqlDatabase::database("central");
    modelTerminals = new QSqlQueryModel();
    modelTerminals->setQuery("SELECT t.terminal_id, TRIM(t.name) AS name, c.SERVER_NAME, c.DB_NAME, c.CON_PASSWORD FROM TERMINALS t "
                             "LEFT JOIN CONNECTIONS c ON t.TERMINAL_ID = c.TERMINAL_ID "
                             "WHERE t.TERMINALTYPE=3 and t.ISACTIVE='T' and c.CONNECT_ID=2 "
                             "ORDER BY t.TERMINAL_ID",dbcenter);
}



void MainWindow::on_lineEditTerminal_textChanged(const QString &arg1)
{
    if(arg1.length()<4)
    {
        ui->labelTerminlInfo->setText("Терминал не указан!");
        ui->groupBoxCheckType->hide();
    }
    else
    {
        int terminal = arg1.toInt();
        int idx = -1;
        QString strTermInfo = "Не верный номер терминала!";
        for(int i=0;i<modelTerminals->rowCount();++i)
        {
            if(terminal == modelTerminals->data(modelTerminals->index(i,0)).toInt())
            {
                idx=i;
                break;
            }
        }
        if(idx>=0)
        {
            strTermInfo=modelTerminals->data(modelTerminals->index(idx,1)).toString();
            currentTerminal=terminal;
            if(validateAZS(idx)) {
                ui->groupBoxCheckType->show();
            } else {
                ui->groupBoxCheckType->hide();
            }

        }
        else
        {
            ui->line->hide();
            ui->labelOnLineStatus->hide();
            ui->groupBoxCheckType->hide();
        }
        ui->labelTerminlInfo->setText(strTermInfo);
    }

}

void MainWindow::on_toolButtonSelectTerminals_clicked()
{
    TerminalListDialog *termListDlg = new TerminalListDialog(modelTerminals);
    connect(termListDlg,&TerminalListDialog::sendTerminalID,this,&MainWindow::getSelectedTerminal);
    termListDlg->exec();
}

void MainWindow::getSelectedTerminal(int termID)
{
    lostCheck["TERMINAL_ID"]=termID;
//    currentTerminal=termID;
    ui->lineEditTerminal->setText(QString::number(termID));

}
