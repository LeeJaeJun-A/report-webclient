#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //socket_.readyRead();
    isChecked_ = false;
    QObject::connect(&sslSocket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&sslSocket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&sslSocket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::doReadyRead(){
    if(isChecked_)
        ui->pteMessage->insertPlainText(sslSocket_.readAll());
    else
        ui->pteMessage->insertPlainText(tcpSocket_.readAll());

}

void Widget::doDisconnected(){
    ui->pteMessage->insertPlainText("Disconnected");
}

void Widget::doConnected(){
    ui->pteMessage->insertPlainText("Connected \r\n");
}

void Widget::on_pbConnect_clicked()
{
    if(isChecked_)
        sslSocket_.connectToHostEncrypted(ui->leHost->text(),ui->lePort->text().toUShort());//SSL needs Encrypted
    else
        tcpSocket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort());
    ui->pbConnect->setEnabled(false);
    ui->pbDisconnect->setEnabled(true);
    ui->leHost->setEnabled(false);
    ui->lePort->setEnabled(false);
    ui->cbSSL->setEnabled(false);
}

void Widget::on_pbDisconnect_clicked()
{
    if(isChecked_)
        sslSocket_.close();
    else
        tcpSocket_.close();
    ui->pbConnect->setEnabled(true);
    ui->pbDisconnect->setEnabled(false);
    ui->leHost->setEnabled(true);
    ui->lePort->setEnabled(true);
    ui->cbSSL->setEnabled(true);
}


void Widget::on_pbSend_clicked()
{
    if(isChecked_)
        sslSocket_.write(ui->pteSend->toPlainText().toUtf8());
    else
        tcpSocket_.write(ui->pteSend->toPlainText().toUtf8());

}


void Widget::on_cbSSL_stateChanged(int arg1)
{
    if(ui->cbSSL->isChecked()){
        isChecked_ = true;
    }else{
        isChecked_ = false;
    }
}


void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}

