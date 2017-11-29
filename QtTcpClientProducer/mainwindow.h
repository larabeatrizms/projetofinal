#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int idtim;
    void timerEvent(QTimerEvent *e);

public slots:
    void Start();
    void tcpConnect();
    void Stop();
    void Disconnect();



private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H
