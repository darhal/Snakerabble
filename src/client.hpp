#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QTimer>
#include <QUdpSocket>
#include <qqml.h>
#include "snakedata.hpp"

class Client : public QObject
{
    Q_OBJECT
    // QML_ELEMENT
    Q_PROPERTY(SnakeData* snakeData READ getSnakeData NOTIFY snakeDataChanged)
    static Client* s_Client;
public:
    static Client* getClient();

    Client();

    Q_INVOKABLE void joinGame();

    SnakeData* getSnakeData() { return &snakeData; }

private slots:
    void processPendingDatagrams();

signals:
    void snakeDataChanged();

private:
    QUdpSocket* udpSocket = nullptr;
    SnakeData snakeData;
};

#endif // CLIENT_HPP
