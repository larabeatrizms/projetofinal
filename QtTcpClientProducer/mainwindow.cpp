#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTextBrowser>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    idtim=0;
    ui->lcdrange->display(ui->sliderrange->value());
    ui->labeltiming->setText(""+ui->slidertiming->value());

    /*!
     * \brief connect Funções de Conexão com cada sinal e slot.
     */
    connect(ui->start,
            SIGNAL(clicked(bool)),
            this,
            SLOT(Start()));

    connect(ui->connect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));


    connect(ui->disconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(Disconnect()));


    connect(ui->stop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(Stop()));

}

/*!
 * \brief
 * Conexão com o servidor
 *
 */
void MainWindow::tcpConnect(){
    /*!
      * \brief Conecta à um servidor Host.
      * \param Nenhum
      * \return Não tem retorno. Função void.
      */
    socket->connectToHost((ui->textip->text()),1234);
    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
    }
    else{
        qDebug() << "Disconnected";
    }
}

void MainWindow::timerEvent(QTimerEvent *e){
    QDateTime datetime;

    QString str;
    qint64 msecdate;

    double min = ui->slidermin->value();
    double range = ui->sliderrange->value();

    int irange=int(range);
    int imin=int(min);

  /*!
   * \brief Primeiramente é declarado as variáveis que os Sliders retornarão.
   * A partir disso(mínimo e range) é produzido os dados no random juntamente com o data.
   * \param Ponteiro qualquer.
   * \return Não tem retorno. Função void.
  */
    if(socket->state()== QAbstractSocket::ConnectedState){

        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
        str = "set "+ QString::number(msecdate) + " " + QString::number(imin + qrand()%irange)+"\r\n";
        ui->plainTextip->insertPlainText(str);

        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
        if(socket->waitForBytesWritten(3000)){
            qDebug() << "wrote";
        }
    }
}

/*!
 * \brief
 *  Começa a produzir os dados.
 */

void MainWindow::Start(){
    /*!
      * \brief Primeiramente é declarada a variável retornada pelo SliderTiming.
      * Precisando fazer um tratamento para que se já estiver começado o Timer é preciso "mata-lo" e começar outro.
      * \param Nenhum
      * \return Não tem retorno. Função void.
      */
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
 * \brief
 * Para a conexão com o servidor.
 */

void MainWindow::Stop(){
    /*!
      * \brief "Mata" o Timer.
      * \param Nenhum
      * \return Não tem retorno. Função void.
      */
    if(idtim!=0){
        killTimer(idtim);
    }
}

/*!
 * \brief
 *  Disconectar com o servidor.
 */
void MainWindow::Disconnect()
{
    /*!
      * \brief Se desconecta do Host.
      * \param Nenhum
      * \return Não tem retorno. Função void.
      */
    socket->disconnectFromHost();
    ui->textip->clear();
}

/*!
 * \brief MainWindow::~MainWindow
 * Destrutor.
 */
MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}
