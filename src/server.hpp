#ifndef SERVER_HPP
#define SERVER_HPP

#include <QTimer>
#include <QUdpSocket>
#include <qqml.h>
#include "commands.hpp"
#include "foodspawner.hpp"

class Client;

class Server : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FoodSpawner* foodSpawner READ getFoodSpawner NOTIFY foodSpawnerChanged)
    static Server* s_Server;
public:
    static Server* getServer();

    Server();

    Q_INVOKABLE void init(uint gridWidth, uint gridHeight);

    Q_INVOKABLE void broadcastGameData();

    Q_INVOKABLE void hostGame();

    Q_INVOKABLE void startGame(uint n = 10, bool local = true);

    Q_INVOKABLE bool isRunning() { return udpSocket != nullptr; }

    Q_INVOKABLE bool isLocal() { return local; }

    Q_INVOKABLE QString getLocalIp();

    Q_INVOKABLE QHostAddress getIpFromBase64(const QString& base64);

    void handleEat(SnakeData& sd, const FoodData& old, uint pid = UINT_MAX);

    void handleCommands(Command& cmd, const QHostAddress& adr, quint16 port);

    template<typename T>
    void sendCommand(const T& cmdData, const QHostAddress& adr = QHostAddress::Broadcast, quint16 port = 45454);

    const QVector<ClientData>& getClientsData() { return m_Clients; }

    Point getGridSize() const { return Point{gridWidth, gridHeight}; }

    FoodSpawner* getFoodSpawner() { return &foodSpawner; }

    ClientData getHostData();

    void setLocal(bool l) { local = l; }
signals:
    void foodSpawnerChanged();

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket* udpSocket = nullptr;
    FoodSpawner foodSpawner;
    QVector<ClientData> m_Clients;
    uint gridWidth, gridHeight;
    bool local = true;
};

#endif // SERVER_HPP
