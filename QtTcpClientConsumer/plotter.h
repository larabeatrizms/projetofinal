#ifndef PLOTTER_H
#define PLOTTER_H
#include <vector>
#include <QWidget>

using namespace std;

class Plotter : public QWidget
{
  Q_OBJECT
private:
    vector<double> t;
    vector<double> d;
public:
  explicit Plotter(QWidget *parent = 0);
  void paintEvent(QPaintEvent *e);
  void loadData(vector<double>, vector<double>);

};

#endif // PLOTTER_H
