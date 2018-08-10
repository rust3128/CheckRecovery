#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

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

signals:
    void sigShowEvent();
protected:
    void showEvent(QShowEvent *ev);
private:
    void setModelConnect();             //Создание модели списка соединений
    int selectCentralDB();              //получение текущего подключения
    bool connCentralDB(int connID);     //Подключение к ЦБ
private:
    Ui::MainWindow *ui;
    QSqlTableModel *modelConnect;       //Модель списка соединений
};

#endif // MAINWINDOW_H
