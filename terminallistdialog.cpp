#include "terminallistdialog.h"
#include "ui_terminallistdialog.h"

TerminalListDialog::TerminalListDialog(QSqlQueryModel *modTerm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TerminalListDialog)
{
    ui->setupUi(this);
    modelTerminals = modTerm;
    createUI();

}

TerminalListDialog::~TerminalListDialog()
{
    delete ui;
}

void TerminalListDialog::createUI()
{
    modelTerminals->setHeaderData(0,Qt::Horizontal,tr("АЗС"));
    modelTerminals->setHeaderData(1,Qt::Horizontal,tr("Расположение"));

    ui->tableView->setModel(modelTerminals);
    ui->tableView->verticalHeader()->hide();
    for(int i = 2;i<=4; ++i)
        ui->tableView->hideColumn(i);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->tableView->verticalHeader()->minimumSectionSize());

}

void TerminalListDialog::on_pushButtonCancel_clicked()
{
    this->reject();
}



void TerminalListDialog::on_tableView_doubleClicked(const QModelIndex &idx)
{
    terminalID = modelTerminals->data(modelTerminals->index(idx.row(),0)).toInt();
    emit sendTerminalID(terminalID);
    this->accept();
}

void TerminalListDialog::on_pushButtonChoise_clicked()
{
    QModelIndex idx;
    idx = ui->tableView->currentIndex();
    terminalID = modelTerminals->data(modelTerminals->index(idx.row(),0)).toInt();
    emit sendTerminalID(terminalID);
    this->accept();
}
