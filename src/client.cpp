#include "client.hpp"

Client* Client::s_Client = nullptr;

Client* Client::getClient() {
    if (!s_Client)
        return s_Client = new Client();
    return s_Client;
}


Client::Client()
{
    snakeData << SnakePieceData{20, 20, 'A'};
    snakeData << SnakePieceData{20, 21, 'B'};
    snakeData << SnakePieceData{20, 22, 'C'};
    snakeData << SnakePieceData{21, 22, 'D'};
    snakeData << SnakePieceData{22, 22, 'E'};
}

void Client::joinGame()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Client::processPendingDatagrams);
    qDebug() << "Joining game";
}

void Client::processPendingDatagrams()
{
    QByteArray datagram;

    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << "Received datagram: " << datagram.constData();
        QDataStream stream(datagram);
        // stream.setByteOrder(QDataStream::BigEndian);
        stream >> snakeData;
        qDebug() << snakeData;
        // emit snakeDataChanged();
    }
}
