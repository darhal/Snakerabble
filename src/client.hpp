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
    Q_PROPERTY(QVector<ClientData> otherPlayers READ getOtherPlayers NOTIFY otherPlayersChanged)
    static Client* s_Client;
public:
    static Client* getClient();

    Client(uint id = UINT_MAX);

    Q_INVOKABLE void eat(uint x, uint y);

    Q_INVOKABLE void joinGame(const QString& name = "Test", const QHostAddress& ip = QHostAddress::Broadcast, quint16 port = 45454);

    Q_INVOKABLE void sendGameData();

    template<typename T>
    void sendCommand(const T& cmdData);

    void handleCommands(Command& cmd);

    SnakeController* getSnakeConroller() { return &snakeController; }

    const QVector<ClientData>& getOtherPlayers() const { return otherPlayers; }

    void setId(uint id) { myId = id; }

    void setOtherPlayers(const QVector<ClientData> op) {
        otherPlayers = op;
        emit otherPlayersChanged();
    }

    const QString& getName() const { return name; }

private slots:
    void processPendingDatagrams();

signals:
    void snakeDataChanged();

    void otherPlayersChanged();

    void popupScoreAnimation(const QString& word, uint combo, uint score);

    void resetAnimation();

    void death();

private:
    QUdpSocket* udpSocket = nullptr;
    SnakeController snakeController;
    QString name;
    QVector<ClientData> otherPlayers;
    QHostAddress serverIp;
    quint16 port;
    uint myId;
};

#endif // CLIENT_HPP
