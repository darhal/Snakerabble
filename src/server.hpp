#ifndef SERVER_HPP
#define SERVER_HPP

#include <QTimer>
#include <QUdpSocket>
#include <qqml.h>
#include "commands.hpp"

class Client;

class Server : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    static Server* s_Server;
public:
    static Server* getServer();

    Server();

    void handleCommands(Command& cmd);

    Q_INVOKABLE void broadcastGameData();

    Q_INVOKABLE void hostGame();

    Q_INVOKABLE bool isRunning() { return udpSocket != nullptr; }

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket* udpSocket = nullptr;
    QVector<Client*> m_Clients;
};

#endif // SERVER_HPP
