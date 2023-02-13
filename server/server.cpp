#include "server.h"

Server::Server()
{
   nextBlockSize = 0;

    #ifdef CRYPTO
    EVP_PKEY * privKey = generate_key();
    if (privKey == NULL)
    {
        exit(-1);
    }
    X509 * certif = generate_x509(privKey);
    if(certif == NULL)
    {
        exit(-1);
    }
    BIO* bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_X509(bio, certif))
    {
        exit(-1);
    }
    BUF_MEM* biostruct;
    BIO_get_mem_ptr(bio, &biostruct);
    std::unique_ptr<char[]> buf = std::make_unique<char[]>(biostruct->length);
    if (static_cast<size_t>(BIO_read(bio, buf.get(), biostruct->length)) != biostruct->length)
    {
        exit(-1);
    }
    QSslCertificate cert(QByteArray(buf.get(), biostruct->length));
    QSslKey key(reinterpret_cast<Qt::HANDLE>(privKey));
    server = new SslServer(this);
    SslConfig = server->sslConfiguration();
    SslConfig.setLocalCertificate(cert);
    SslConfig.setPrivateKey(key);
    server->setSslConfiguration(SslConfig);
    #endif // CRYPTO
}

void Server::incomingConnection(qintptr socketDescriptor)
{
   Socket = new QSslSocket;
   Socket->setSocketDescriptor(socketDescriptor);

   connect(Socket, &QSslSocket::readyRead, this, &Server::slotReadyRead);
   connect(Socket, &QSslSocket::disconnected, this, &Server::slotDisconnected);

   sockets.push_back(Socket);

   qDebug() << Socket->peerAddress().toString();
   qDebug() << "Amount of users: " << sockets.size();

   emit clientConnected(Socket);
}

void Server::slotDisconnected()
{
   Socket = (QSslSocket*)sender();
   for(int i = 0; i < sockets.size(); i++)
   {
       if(Socket->socketDescriptor() == sockets[i]->socketDescriptor())
       {
           sockets.removeAt(i);
           emit clientDisconnected(Socket);
           break;
       }
   }
}

void Server::slotReadyRead()
{
    nextBlockSize = 0;
    Socket = (QSslSocket*)sender();
    QDataStream in(Socket);

    if(in.status() == QDataStream::Ok)
    {
        while(true){
            if(nextBlockSize == 0)
            {
                if(Socket->bytesAvailable() < 2) break;
                in >> nextBlockSize;
            }
            if(nextBlockSize > Socket->bytesAvailable()) break;

            QString com;
            in >> com;
            qDebug() << com;
            if(com == "newMsg"){
                QString str, name, ip;
                in >> name >> str >> ip;
                nextBlockSize = 0;
                sendToClient("newMsg", name, str, ip);
                break;
            }
            else if(com == "newUser"){
                QString ip, port, username, status, avatarFile;
                in >> ip >> port >> username >> status >> avatarFile;
                clients.push_back(new Client(ip, port.toInt(), username, status, avatarFile, Socket));
                nextBlockSize = 0;
                sendToClient("newUser");
                break;
            }
            else if(com == "exitUser"){
                QString username;
                in >> username;
                for(int i = 0; i < clients.size(); i++){
                     if(username == clients[i]->username){
                        clients.removeAt(i);
                        break;
                     }
                }
                nextBlockSize = 0;
                sendToClient("exitUser");
                break;
            }
            else if(com == "newImg"){
                QString str, name, ip;
                in >> name >> str >> ip;
                nextBlockSize = 0;
                sendToClient("newImg", name, str, ip);
                break;
            }
            else if(com == "newFile"){
                qDebug() << "getting file";
                QString fileName, fileSize, name, ip;
                int curFileSize = 0;
                QString filePath = "D:\\QT\\V_lab\\messenger-main\\server\\downloads\\";
                in >> name >> ip >> fileSize >> fileName;
                qDebug() << fileName;
                QFile file(filePath + fileName);
                if(file.open(QIODevice::WriteOnly)){
                    QDataStream sendFile(&file);
                    while(curFileSize < fileSize.toInt()){
                        QByteArray line = Socket->readAll();
                        curFileSize += sendFile.writeRawData(line.data(), line.size());
                        Socket->waitForReadyRead(100);
                        qDebug() << curFileSize << " vs " << fileSize << " |" << line.size();
                    }
                }
                else qDebug() << "can't get";

                file.close();

                //nextBlockSize = 0;
                sendToClient("newFile", name, file.fileName() , ip);
                break;
            }
        }
    }
    else qDebug() << "error of datastream";
}

void Server::sendToClient(QString com, QString name,  QString str, QString ip){
    data.clear();

    QDataStream out(&data, QIODevice::WriteOnly);

    if(com == "newMsg"){
        out << quint16(0) << com;
        out << name << str << QDate::currentDate().toString() << QTime::currentTime().toString() << ip;
        out.device()->seek(0);
        out << quint16(data.size() - sizeof(quint16));
    }
    else if(com == "newUser"){
        out << quint16(0) << com << QString::number(clients.size());

        for(int i = 0; i < clients.size(); i++){
            out << clients[i]->ip << QString::number(clients[i]->port) << clients[i]->username << clients[i]->status << clients[i]->ava_File;
        }
        out.device()->seek(0);
        out << quint16(data.size() - sizeof(quint16));
    }
    else if(com == "exitUser"){
        out << quint16(0) << com << QString::number(clients.size());

        for(int i = 0; i < clients.size(); i++){
            out << clients[i]->ip << QString::number(clients[i]->port) << clients[i]->username << clients[i]->status;
        }
        out.device()->seek(0);
        out << quint16(data.size() - sizeof(quint16));
    }
    else if(com == "newImg"){
        out << quint16(0) << com;
        out << name << str << QDate::currentDate().toString() << QTime::currentTime().toString() << ip;
        out.device()->seek(0);
        out << quint16(data.size() - sizeof(quint16));
    }
    else if(com == "restart"){
        out << quint16(0) << com;
        out.device()->seek(0);
        out << quint16(data.size() - sizeof(quint16));
        for(int i = 0; i < clients.size(); i++){
            emit clientDisconnected(clients[i]->socket);
        }
        clients.clear();
    }
    else if(com == "newFile"){
        out << quint16(0) << com;
        QFile file(str);
        QString fileName(str.split("/").last());
        QDataStream sendFile(&file);
        if (file.open(QIODevice::ReadOnly)) {
            qDebug() << "opened file";
            out << name << ip << QTime::currentTime().toString() << QString::number(file.size()) << fileName;
            out.device()->seek(0);
            out << quint16(data.size() - sizeof(quint16));
            for(int i = 0; i < sockets.size(); i++){
                sockets[i]->write(data);
                Socket->waitForReadyRead(100);
                int curSendSize = 0;
                char bytes[8];
                bytes[7] = '\0';
                while (curSendSize < file.size()) {
                    int lenght = sendFile.readRawData(bytes, sizeof(char) * 7);
                    data.setRawData(bytes, sizeof(char) * lenght);
                    curSendSize += sockets[i]->write(data, sizeof(char) * lenght);
                    Socket->waitForReadyRead(100);
                    qDebug() << curSendSize << " " << file.size() << " |" << lenght;
                }
            }
            file.close();
        }
        out.device()->seek(0);
        out << quint16(data.size() - sizeof(quint16));
        data.clear();
    }
    for(int i = 0; i < sockets.size(); i++)
    {
        sockets[i]->write(data);
    }
}


int Server::getUsersCount(){
    return sockets.size();
}
