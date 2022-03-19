#include "server.hpp"
#include "snakedata.hpp"
#include "client.hpp"

Server::Server()
{
}

void Server::broadcastGameData()
{
    // QByteArray datagram = "Broadcast message " + QByteArray::number(messageNo++);
    QByteArray bytes;
    QDataStream ds(&bytes, QIODeviceBase::WriteOnly);
    ds << *Client::getClient()->getSnakeData();
    udpSocket->writeDatagram(bytes, QHostAddress::Broadcast, 45454);
    qDebug() << "Sending broadcast";
}

void Server::receiveGameData()
{

}

void Server::hostGame()
{
    udpSocket = new QUdpSocket(this);
    qDebug() << "Hosting game";
}

