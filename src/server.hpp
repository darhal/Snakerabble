#ifndef SERVER_HPP
#define SERVER_HPP

#include <QTimer>
#include <QUdpSocket>
#include <qqml.h>

class Server : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    Server();

    void receiveGameData();

    Q_INVOKABLE void broadcastGameData();

    Q_INVOKABLE void hostGame();

private:
    QUdpSocket* udpSocket = nullptr;
};

#endif // SERVER_HPP
