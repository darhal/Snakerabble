#include "server.hpp"
#include "snakedata.hpp"
#include "client.hpp"
#include "commands.hpp"
#include <QNetworkInterface>

Server* Server::s_Server = nullptr;

Server* Server::getServer()
{
    if (!s_Server)
        return s_Server = new Server();
    return s_Server;
}

Server::Server()
{
    connect(&foodSpawner, &FoodSpawner::foodDataChanged, this, &Server::foodSpawnerChanged);
}

void Server::init(uint gridWidth, uint gridHeight)
{
    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;
    // foodSpawner.init(10);
}


void Server::handleEat(SnakeData& sd, const FoodData& old, uint pid)
{
    RespawnFood rfcmd;
    rfcmd.toDelete = old;
    rfcmd.food = foodSpawner.respawn(rfcmd.toDelete, &rfcmd.idx);
    rfcmd.pid = pid;

    if (rfcmd.food.letter != QChar(0)) {
        auto eres = sd.eat(old.x, old.y, old.letter);
        TrieNode* nextTrieNode = Trie::get()->next(sd.trieNode, old.letter);

        if (eres == 1 && pid == UINT_MAX) {
            emit Client::getClient()->popupScoreAnimation(sd.word, sd.combo, sd.score);
        }else if (eres == 2 && pid == UINT_MAX) {
            emit Client::getClient()->resetAnimation();
        }

        if (this->isRunning()) {
            this->sendCommand(rfcmd);
        }
    }
}

void Server::handleCommands(Command& cmd, const QHostAddress& adr, quint16 port)
{
    if (cmd.id == Command::JOIN_GAME) {
        JoinGameCmd jgcmd = cmd.getData<JoinGameCmd>();
        qDebug() << jgcmd.name << " (IP:"<<adr<<") has joined the game";
        uint id = m_Clients.size();
        m_Clients.emplaceBack(ClientData{jgcmd.data, jgcmd.name, jgcmd.color, id, adr, port});
        this->sendCommand(JoinGameReply{m_Clients, this->getHostData(), foodSpawner.getFoodData(), id});
        Client::getClient()->setOtherPlayers(m_Clients);
    }else if (cmd.id == Command::SEND_PLAYER_DATA) {
        SendPlayerCmd spcmd = cmd.getData<SendPlayerCmd>();
        m_Clients[spcmd.id].pdata = spcmd.playerData;
        Client::getClient()->setOtherPlayers(m_Clients);
    }else if (cmd.id == Command::CONSUME_FOOD) {
        EatFoodCmd efcmd = cmd.getData<EatFoodCmd>();
        this->handleEat(m_Clients[efcmd.pid].pdata, efcmd.food, efcmd.pid);
    }
}

ClientData Server::getHostData()
{
    const auto& sd = Client::getClient()->getSnakeConroller()->getSnakeData();
    const auto& name = Client::getClient()->getName();
    return ClientData{sd, name, 0, UINT_MAX, QHostAddress::LocalHost, 0};
}

void Server::broadcastGameData()
{
    if (m_Clients.size() == 0)
        return;
    for (const auto& c : m_Clients) {
        this->sendCommand(SyncGameCmd{m_Clients, this->getHostData()}, c.adr, c.port);
    }
}

void Server::hostGame()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::processPendingDatagrams);
    qDebug() << "Hosting game";
}

void Server::startGame(uint n, bool local)
{
    foodSpawner.init(n);
    this->local = local;
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

QString Server::getLocalIp()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    QHostAddress host;

    for(int nIter=0; nIter<list.count(); nIter++) {
        if(!list[nIter].isLoopback() && list[nIter].isGlobal()/*&& list[nIter].protocol() == QAbstractSocket::IPv4Protocol*/)
            host = list[nIter];
        //qDebug() << list[nIter];
    }

    //host = QHostAddress("81.220.35.128");
    //qDebug() << host << host.toString().toUtf8().toBase64();
    return host.toString().toUtf8().toBase64();
}

QHostAddress Server::getIpFromBase64(const QString& base64)
{
    QString ip(QByteArray::fromBase64(base64.toUtf8(), QByteArray::Base64Encoding));
    return QHostAddress(ip);
}
