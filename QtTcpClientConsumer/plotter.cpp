#include "plotter.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <cmath>
#include <QDebug>
#include <QMouseEvent>

#define namostra 30

using namespace std;

/*!
 * \brief Plotter::Plotter
 * \param parent
 */
Plotter::Plotter(QWidget *parent) :
    QWidget(parent)
{
     for(int i=0;i<namostra;i++){
        t.push_back(i*1.0/29);
        d.push_back(i*1.0/29);
    }
}

/*!
 * \brief Plotter::paintEvent Processo de desenhar o gráfico.
 * \param e
 */
void Plotter::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QBrush brush;
    QPen pen;
    int x1, x2, y1, y2;

    painter.setRenderHint(QPainter::Antialiasing);

    brush.setColor(QColor(255,255,200));
    brush.setStyle(Qt::SolidPattern);

    pen.setColor(QColor(255,0,0));
    pen.setWidth(3);

    painter.setBrush(brush);
    painter.setPen(pen);

    // desenha o fundo do plotter
    painter.drawRect(0,0,width(), height());

    // desenha o grid
    pen.setColor(QColor(0,0,0));
    pen.setWidth(3);
    painter.setPen(pen);


    x1=t[0]*width();
    y1=d[0]*(height()-d[0]);


    for(int i=1;i<namostra;i++){
        x2=t[i]*width();
        y2=d[i]*(height()-d[i]);
        painter.drawLine(x1,y1,x2,y2);
        x1=x2;
        y1=y2;
    }


}
/*!
 * \brief Plotter::loadData Redesenhando.
 * \param tem
 * \param dad
 */
void Plotter::loadData(vector<double> tem, vector<double> dad)
{
    for(int i=0;i<namostra;i++){
        t[i]=tem[i];
        d[i]=dad[i];
    }
    repaint();
}

