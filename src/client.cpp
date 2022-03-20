#include "client.hpp"

Client* Client::s_Client = nullptr;

Client* Client::getClient()
{
    if (!s_Client)
        return s_Client = new Client();
    return s_Client;
}


Client::Client()
{
    snakeController << SnakePieceData{20, 20, 'A'};
    snakeController << SnakePieceData{20, 21, 'B'};
    snakeController << SnakePieceData{20, 22, 'C'};
    snakeController << SnakePieceData{21, 22, 'D'};
    snakeController << SnakePieceData{22, 22, 'E'};
}

void Client::joinGame()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Client::processPendingDatagrams);
    JoinGameCmd jgcmd{"Test", this->getSnakeConroller()->getSnakeData(), 1234};
    Command cmd(jgcmd);
    udpSocket->writeDatagram(cmd.getBytes(), QHostAddress::Broadcast, 45454);
}

void Client::sendGameData()
{
    QByteArray bytes;
    QDataStream ds(&bytes, QIODeviceBase::WriteOnly);
    SendPlayerCmd spcmd{this->getSnakeConroller()->getSnakeData()};
    Command cmd{spcmd};
    udpSocket->writeDatagram(cmd.getBytes(), QHostAddress::Broadcast, 45454);
}

void Client::handleCommands(Command& cmd)
{
    if (cmd.id == Command::JOIN_GAME) {
        JoinGameCmd jgcmd = cmd.getData<JoinGameCmd>();
        qDebug() << jgcmd.name << " has joined the game";
    }else if (cmd.id == Command::BROADCAST_SYNC){
        SyncGameCmd sgmd = cmd.getData<SyncGameCmd>();
        otherPlayers = sgmd.playersData;
    }
}

void Client::processPendingDatagrams()
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
