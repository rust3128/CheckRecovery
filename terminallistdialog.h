#ifndef TERMINALLISTDIALOG_H
#define TERMINALLISTDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class TerminalListDialog;
}

class TerminalListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TerminalListDialog(QSqlQueryModel *modTerm, QWidget *parent = 0);
    ~TerminalListDialog();

signals:
    void sendTerminalID(int);
private slots:
    void on_pushButtonCancel_clicked();
    void on_tableView_doubleClicked(const QModelIndex &idx);

    void on_pushButtonChoise_clicked();

private:
    void createUI();
private:
    Ui::TerminalListDialog *ui;
    QSqlQueryModel *modelTerminals;
    int terminalID;
};

#endif // TERMINALLISTDIALOG_H
