#include "client.hpp"
#include "server.hpp"
#include "foodspawner.hpp"

Client* Client::s_Client = nullptr;

Client* Client::getClient()
{
    if (!s_Client)
        return s_Client = new Client();
    return s_Client;
}

Client::Client(uint id) : myId(id)
{
    /*snakeController << SnakePieceData{20, 20, QChar(FoodSpawner::random(0,26)+'A')};
    snakeController << SnakePieceData{20, 21, QChar(FoodSpawner::random(0,26)+'A')};
    snakeController << SnakePieceData{20, 22, QChar(FoodSpawner::random(0,26)+'A')};
    snakeController << SnakePieceData{21, 22, QChar(FoodSpawner::random(0,26)+'A')};
    snakeController << SnakePieceData{22, 22, QChar(FoodSpawner::random(0,26)+'A')};*/

    snakeController << SnakePieceData{23, 22, QChar('H')};
    snakeController << SnakePieceData{22, 22, QChar('E')};
    snakeController << SnakePieceData{21, 22, QChar('L')};
    snakeController << SnakePieceData{20, 22, QChar('L')};
    snakeController << SnakePieceData{19, 22, QChar('O')};
    snakeController << SnakePieceData{18, 22, QChar('H')};
    snakeController << SnakePieceData{17, 22, QChar('O')};
    snakeController << SnakePieceData{16, 22, QChar('W')};
    snakeController << SnakePieceData{15, 22, QChar('A')};
    snakeController << SnakePieceData{14, 22, QChar('R')};
    snakeController << SnakePieceData{13, 22, QChar('E')};
    snakeController << SnakePieceData{12, 22, QChar('Y')};
    snakeController << SnakePieceData{11, 22, QChar('O')};
    snakeController << SnakePieceData{10, 22, QChar('U')};
}

void Client::eat(uint x, uint y)
{
    auto server = Server::getServer();
    auto foodSpawner = server->getFoodSpawner();
    auto food = foodSpawner->get({x, y});

    if (food.letter == QChar(0))
        return;

    if (server->isRunning() || myId == UINT_MAX) {
        server->handleEat(snakeController.getSnakeData(), food, UINT_MAX);
    }else{
        this->snakeController.eat(food.x, food.y, food.letter);
        this->sendCommand(EatFoodCmd{food, (uint)myId});
    }
}

void Client::joinGame(const QString& name, const QHostAddress& ip, quint16 port)
{
    qDebug() << ip;
    Server::getServer()->setLocal(false);
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Client::processPendingDatagrams);
    this->serverIp = ip;
    this->port = port;
    this->name = name;
    this->sendCommand(JoinGameCmd{name, this->getSnakeConroller()->getSnakeData(), 1234});
}

void Client::sendGameData()
{
    if (Server::getServer()->isRunning() || myId == UINT_MAX) // I'm server or I'm in SP
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
            this->otherPlayers = std::move(jgr.playersData);
            this->otherPlayers.emplaceBack(std::move(jgr.hostData));
            qDebug() << "You have joined the game with id = " << jgr.id;
            Server::getServer()->getFoodSpawner()->init(jgr.foodData);
            emit otherPlayersChanged();
        }
    }else if (cmd.id == Command::BROADCAST_SYNC){
        if (myId == UINT_MAX) return;

        if (Server::getServer()->isRunning()) {
            otherPlayers = Server::getServer()->getClientsData();
            emit otherPlayersChanged();
            return;
        }

        SyncGameCmd sgmd = cmd.getData<SyncGameCmd>();
        // this->snakeController.setData(otherPlayers[myId].pdata);
        otherPlayers = std::move(sgmd.playersData);
        otherPlayers.erase(otherPlayers.begin() + myId);
        this->otherPlayers.emplaceBack(std::move(sgmd.hostData));
        emit otherPlayersChanged();
    }else if (cmd.id == Command::RESPAWN_FOOD) {
        auto rfcmd = cmd.getData<RespawnFood>();

        if (rfcmd.pid == myId) {
            qDebug() << "CONFIRM : I ate at " << rfcmd.toDelete.x << "," << rfcmd.toDelete.y;
            // this->snakeController.eat(rfcmd.toDelete.x, rfcmd.toDelete.y, rfcmd.toDelete.letter);
        }else{
            if (rfcmd.pid != UINT_MAX)
                otherPlayers[rfcmd.pid].pdata.eat(rfcmd.toDelete.x, rfcmd.toDelete.y, rfcmd.toDelete.letter);
        }

        Server::getServer()->getFoodSpawner()->destroy({rfcmd.toDelete.x, rfcmd.toDelete.y});
        Server::getServer()->getFoodSpawner()->spawn(rfcmd.idx, rfcmd.food);
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
