#include "server.hpp"
#include "snakedata.hpp"
#include "client.hpp"
#include "commands.hpp"

Server* Server::s_Server = nullptr;

Server* Server::getServer()
{
    if (!s_Server)
        return s_Server = new Server();
    return s_Server;
}

Server::Server()
{
}

void Server::handleCommands(Command& cmd)
{
    if (cmd.id == Command::JOIN_GAME) {
        JoinGameCmd jgcmd = cmd.getData<JoinGameCmd>();
        qDebug() << jgcmd.name << " has joined the game";
        auto id = Client::getClient()->otherPlayers.size();
        Client::getClient()->otherPlayers.emplaceBack();
    }else if (cmd.id == Command::BROADCAST_SYNC){

    }
}

void Server::broadcastGameData()
{
    QByteArray bytes;
    QDataStream ds(&bytes, QIODeviceBase::WriteOnly);
    ds << *Client::getClient()->getSnakeConroller();
    udpSocket->writeDatagram(bytes, QHostAddress::Broadcast, 45454);
    // qDebug() << "Sending broadcast";
}

void Server::hostGame()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::processPendingDatagrams);
    qDebug() << "Hosting game";
}

void Server::processPendingDatagrams()
{
    QByteArray datagram;

    while (udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(datagram.data(), datagram.size());
        Command cmd{datagram};
        this->handleCommands(cmd);
        qDebug() << "Received datagram: " << datagram.constData();
    }
}

