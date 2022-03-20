#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QTimer>
#include <QUdpSocket>
#include <qqml.h>
#include "snakedata.hpp"
#include "commands.hpp"

class Client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SnakeController* snakeController READ getSnakeConroller NOTIFY snakeDataChanged)
    static Client* s_Client;
public:
    static Client* getClient();

    Client();

    Q_INVOKABLE void joinGame();

    void sendGameData();

    SnakeController* getSnakeConroller() { return &snakeController; }

    void handleCommands(Command& cmd);

private slots:
    void processPendingDatagrams();

signals:
    void snakeDataChanged();

private:
    QUdpSocket* udpSocket = nullptr;
    SnakeController snakeController;
    QVector<SnakeData> otherPlayers;
    friend class Server;
};

#endif // CLIENT_HPP
