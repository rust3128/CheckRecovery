#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QLabel>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void slotShowEvent();
    void on_action_triggered();

    void on_lineEditTerminal_textChanged(const QString &arg1);

signals:
    void sigShowEvent();
protected:
    void showEvent(QShowEvent *ev);
private:
    void setModelConnect();             //Создание модели списка соединений
    void setModelTerminals();           //Создание модели списка терминалов
    void initLostCheckFuels();
    int selectCentralDB();              //получение текущего подключения
    bool connCentralDB(int connID);     //Подключение к ЦБ
    void createStatusBar();             //Создание строки состогяния
    bool validateAZS(int idx);                 //Проверка доступности АЗС
private:
    Ui::MainWindow *ui;
    QSqlTableModel *modelConnect;       //Модель списка соединений c центральной базо
    QSqlQueryModel *modelTerminals;     //Модель списка терминалов
    QLabel *statusLabel;                //Информация о текщем подключении
    QMap<QString, QVariant> lostCheck;  //Контейнер данных для формирования скрипта
    int currentTerminal;                //Выбранный терминал
    QTcpSocket  *tcpSocket;             //Проверка доступности FierBird

};

#endif // MAINWINDOW_H
