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

void Server::handleCommands(Command& cmd, const QHostAddress& adr, quint16 port)
{
    if (cmd.id == Command::JOIN_GAME) {
        JoinGameCmd jgcmd = cmd.getData<JoinGameCmd>();
        qDebug() << jgcmd.name << " (IP:"<<adr<<") has joined the game";
        uint id = m_Clients.size();
        m_Clients.emplaceBack(ClientData{jgcmd.data, jgcmd.name, jgcmd.color, id, adr, port});
        auto clientData = m_Clients;
        clientData.emplaceBack(ClientData{Client::getClient()->getSnakeConroller()->getSnakeData(),
                                          "Host", 0, (uint)clientData.size(), QHostAddress::LocalHost, 0});
        this->sendCommand(JoinGameReply{clientData, id});
        Client::getClient()->setOtherPlayers(m_Clients);
    }else if (cmd.id == Command::SEND_PLAYER_DATA) {
        SendPlayerCmd spcmd = cmd.getData<SendPlayerCmd>();
        m_Clients[spcmd.id].pdata = spcmd.playerData;
        Client::getClient()->setOtherPlayers(m_Clients);
    }
}

void Server::broadcastGameData()
{
    if (m_Clients.size() == 0)
        return;
    // TODO: forget to include the host!
    auto clientData = m_Clients;
    clientData.emplaceBack(ClientData{Client::getClient()->getSnakeConroller()->getSnakeData(),
                                      "Host", 0, (uint)clientData.size(), QHostAddress::LocalHost, 0});
    for (const auto& c : m_Clients) {
        this->sendCommand(SyncGameCmd{clientData}, c.adr, c.port);
    }
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
        QHostAddress adr; quint16 port;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &adr, &port);
        Command cmd{datagram};
        this->handleCommands(cmd, adr, port);
        //qDebug() << "[SERVER] Received datagram (IP:" << adr << ",PORT:" << port << ")" << " size = "
        //         << datagram.size() << " data : " << datagram.constData() << " cmd id : " << cmd.id;;
    }
}


template<typename T>
void Server::sendCommand(const T& cmdData, const QHostAddress& adr, quint16 port)
{
    Command cmd{cmdData};

    if (adr == QHostAddress::Broadcast) {
        //qDebug() << "[SERVER] broadcasting command id " << T::ID;

        for (const auto& c : m_Clients) {
            this->sendCommand(cmdData, c.adr, c.port);
        }

        return;
    }

    //qDebug() << "[SERVER] sending command id " << T::ID << " to adr=" << adr << " port=" << port;
    udpSocket->writeDatagram(cmd.getBytes(), adr, port);
}
