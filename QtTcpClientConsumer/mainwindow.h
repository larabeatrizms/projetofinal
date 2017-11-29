#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <vector>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  int idtim=0;
  ~MainWindow();
  void timerEvent(QTimerEvent *e);
  void metNorma(vector<double> a, vector<double> b);

public slots:
  void Start();
  void tcpConnect();
  void Disconnect();
  void Stop();
  void Update();
private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
};

#endif // MAINWINDOW_H
