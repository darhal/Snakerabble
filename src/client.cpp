#include "client.hpp"
#include "server.hpp"

Client* Client::s_Client = nullptr;

Client* Client::getClient()
{
    if (!s_Client)
        return s_Client = new Client();
    return s_Client;
}

Client::Client(int id) : myId(id)
{
    snakeController << SnakePieceData{20, 20, 'A'};
    snakeController << SnakePieceData{20, 21, 'B'};
    snakeController << SnakePieceData{20, 22, 'C'};
    snakeController << SnakePieceData{21, 22, 'D'};
    snakeController << SnakePieceData{22, 22, 'E'};
}

void Client::joinGame(const QString& name, const QHostAddress& ip, quint16 port)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Client::processPendingDatagrams);
    this->serverIp = ip;
    this->port = port;
    this->sendCommand(JoinGameCmd{name, this->getSnakeConroller()->getSnakeData(), 1234});
}

void Client::sendGameData()
{
    if (Server::getServer()->isRunning() || myId == -1) // I'm server or I'm in SP
        return;
    this->sendCommand(SendPlayerCmd{this->getSnakeConroller()->getSnakeData(), (uint)myId});
}

void Client::handleCommands(Command& cmd)
{
    if (cmd.id == Command::JOIN_GAME_REPLY) {
        JoinGameReply jgr = cmd.getData<JoinGameReply>();
        if (jgr.id == (uint)myId || Server::getServer()->isRunning()) {
            qDebug() << jgr.playersData[jgr.id].name << " has joined the game";
        }else{
            this->myId = jgr.id;
            jgr.playersData.pop_back();
            this->otherPlayers = jgr.playersData;
            emit otherPlayersChanged();
            qDebug() << "You have joined the game with id = " << jgr.id;
        }
    }else if (cmd.id == Command::BROADCAST_SYNC){
        if (myId == -1) return;

        if (Server::getServer()->isRunning()) {
            otherPlayers = Server::getServer()->getClientsData();
            emit otherPlayersChanged();
            return;
        }

        SyncGameCmd sgmd = cmd.getData<SyncGameCmd>();
        otherPlayers.erase(otherPlayers.begin() + myId);
        otherPlayers = sgmd.playersData;
        emit otherPlayersChanged();
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
        //qDebug() << "[CLIENT] Received datagram size = " << datagram.size()
        //         << " data : " << datagram.constData() << " cmd id : " << cmd.id;
    }
}

template<typename T>
void Client::sendCommand(const T& cmdData)
{
    Command cmd{cmdData};
    // qDebug() << "[CLIENT] sending " << T::ID << " command";
    udpSocket->writeDatagram(cmd.getBytes(), serverIp, port);
}
