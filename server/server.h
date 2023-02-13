#ifndef SERVER_H
#define SERVER_H

#include <QSslSocket>
#include <QTcpServer>
#include <QVector>
#include <QNetworkInterface>
#include <QDate>
#include <QTime>
#include <QFile>

//#define CRYPTO
#ifdef CRYPTO
#include "QSslKey"
#include "QSslSocket"
#include "QSslCertificate"
#include "openssl_generator_.h"
#endif // CRYPTO

class Client{
public:
    QString ip;
    int port;
    QString username;
    QString status;
    QString ava_File;
    QSslSocket* socket;

    Client();
    Client(QString ip, int port, QString username, QString status, QString avatarFile, QSslSocket* socket)
    {
        this->ip = ip;
        this->port = port;
        this->username = username;
        this->status = status;
        this->ava_File = avatarFile;
        this->socket = socket;
    }
};

extern int SERVER_PORT;

class Server: public QTcpServer {
    Q_OBJECT

public:
    Server();
    QSslSocket* Socket;
    QVector<Client*> clients;

    int getUsersCount();

     void sendToClient(QString com, QString name = "", QString str = "", QString ip = "");

private:
    QVector<QSslSocket*> sockets;
    QByteArray data;
    quint16 nextBlockSize; 

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void slotDisconnected();
signals:
    void clientConnected(QSslSocket* soc);
    void clientDisconnected(QSslSocket* soc);
};

#endif // SERVER_H
