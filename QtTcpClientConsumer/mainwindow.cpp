#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <cmath>
#include <QDebug>
#include <vector>
#include "plotter.h"

#define namostras 30

using namespace std;

/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    idtim=0;

//    ui->labeltiming->setText(""+ui->slidertiming->value());

    /*!
     * \brief connect Conexão com botão Update.
     */
    connect(ui->update,
            SIGNAL(clicked(bool)),
            this,
            SLOT(Update()));

    /*!
     * \brief connect Conexão com o botão Disconnect.
     */
    connect(ui->disconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(Disconnect()));

    /*!
     * \brief connect Conexão com o botão Stop.
     */
    connect(ui->stop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(Stop()));

    /*!
     * \brief connect Conexão com o botão Connect.
     */
    connect(ui->connect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));

    /*!
     * \brief connect Conexão com o botão Start.
     */
    connect(ui->start,
            SIGNAL(clicked(bool)),
            this,
            SLOT(Start()));
}

/*!
 * \brief MainWindow::tcpConnect Conexão com o Host.
 */
void MainWindow::tcpConnect(){
    socket->connectToHost(ui->textip->text(),1234);
    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
    }
    else{
        qDebug() << "Disconnected";
    }
    Update();
}
/*!
 * \brief MainWindow::timerEvent Lendo os dados.
 * \param e
 */
void MainWindow::timerEvent(QTimerEvent *e){
    QString str;
    QByteArray array;
    QStringList list;
    double thetime;
    double dados;
    vector<double> tempo;
    vector<double> dado;

    qDebug() << "to get data...";

    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            qDebug() << "reading...";
            str="get "+ui->lista->currentItem()->text()+" 30\r\n";
            socket->write(str.toStdString().c_str());
            socket->waitForBytesWritten();
            socket->waitForReadyRead();

            qDebug() << socket->bytesAvailable();

            tempo.clear();
            dado.clear();

            while(socket->bytesAvailable()){
                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");
                if(list.size() == 2){
                    bool ok;
                    str = list.at(0);
                    thetime = str.toDouble(&ok);
                    tempo.push_back(thetime);
                    str = list.at(1);
                    dados = str.toDouble(&ok);
                    dado.push_back(dados);

                    qDebug() << thetime << ": " << str;
                }
            }
            metNorma(tempo,dado);

        }
    }
}
/*!
 * \brief MainWindow::metNorma
 * \param a
 * \param b
 */
void MainWindow::metNorma(vector<double> a, vector<double> b)
{
    vector<double> tnorma;
    vector<double> dnorma;
    double tmenor= a[0];
    double dmenor= b[0];
    double t1, d1;

    double tmaior=a[0];
    double dmaior=b[0];

    for(int i=1;i<namostras;i++){
        if(a[i]<tmenor){
            tmenor=a[i];
        }

        if(b[i]<dmenor){
            dmenor=b[i];
        }
        if(a[i]>tmaior){
            tmaior=a[i];
        }

        if(b[i]>dmaior){
            dmaior=b[i];
        }
    }



    for(int i=0;i<namostras;i++){
        t1=(a[i] - tmenor)/(tmaior-tmenor);
        d1=(b[i] - dmenor)/(dmaior-dmenor);
        tnorma.push_back(t1);
        dnorma.push_back(d1);
    }

    ui->plotter->loadData(tnorma,dnorma);

}

/*!
 * \brief MainWindow::Start Start timer de acordo com o slider.
 */
void MainWindow::Start(){
    QString qtim;

    qtim=ui->slidertiming->value();

    if(idtim==0){
        idtim = startTimer(ui->slidertiming->value()*1000);
    }else{
        killTimer(idtim);
        idtim = startTimer(ui->slidertiming->value()*1000);
    }
}

/*!
 * \brief MainWindow::Stop "Matar o tempo."
 */
void MainWindow::Stop(){
    if(idtim!=0){
        killTimer(idtim);
    }
}

/*!
 * \brief MainWindow::Update Update lista de ips.
 */
void MainWindow::Update(){
    QString str;
    QStringList list;
    QByteArray array;

    ui->lista->clear();

    socket->write("list\r\n");
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    while(socket->bytesAvailable()){
        str = socket->readLine().replace("\n","").replace("\r","");
        ui->lista->addItem(str);

    }
}

/*!
 * \brief MainWindow::Disconnect Desconectar com o servidor.
 */
void MainWindow::Disconnect(){
    socket->disconnectFromHost();
    ui->textip->clear();
    ui->lista->clear();

}


/*!
 * \brief MainWindow::~MainWindow
 * Destrutor.
 */
MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
