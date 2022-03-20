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

    void handleCommands(Command& cmd, const QHostAddress& adr, quint16 port);

    template<typename T>
    void sendCommand(const T& cmdData, const QHostAddress& adr = QHostAddress::Broadcast, quint16 port = 45454);

    Q_INVOKABLE void broadcastGameData();

    Q_INVOKABLE void hostGame();

    Q_INVOKABLE bool isRunning() { return udpSocket != nullptr; }

    const QVector<ClientData>& getClientsData() { return m_Clients; }
private slots:
    void processPendingDatagrams();

private:
    QUdpSocket* udpSocket = nullptr;
    QVector<ClientData> m_Clients;
};

#endif // SERVER_HPP
