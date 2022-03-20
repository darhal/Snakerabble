#ifndef CLIENTDATA_HPP
#define CLIENTDATA_HPP

#include <QTimer>
#include <QPoint>
#include <qqml.h>
#include <QHostAddress>
#include "snakedata.hpp"

struct ClientData
{
    Q_GADGET
public:
    Q_PROPERTY(SnakeData pdata READ getPdata)
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(uint color READ getColor)
    Q_PROPERTY(uint id READ getId)
    Q_PROPERTY(QHostAddress adr READ getAdr)
    Q_PROPERTY(quint16 port READ getPort)

    SnakeData pdata;
    QString name;
    uint color;
    uint id;
    QHostAddress adr;
    quint16 port;

    const SnakeData& getPdata() const { return pdata; }
    const QString& getName() const { return name; }
    uint getColor() const { return color; }
    uint getId() const { return id; }
    const QHostAddress& getAdr() const { return adr; }
    quint16 getPort() const { return port; }

    friend inline QDataStream& operator<<(QDataStream& s, const ClientData& data)
    {
        s << data.pdata << data.name << data.color << data.id << data.adr << data.port;
        return s;
    }

    friend inline QDataStream& operator>>(QDataStream& s, ClientData& data)
    {
        s >> data.pdata >> data.name >> data.color >> data.id >> data.adr >> data.port;
        return s;
    }

    friend inline QDebug operator<<(QDebug d, const ClientData& data){
        QDebug nsp = d.nospace();
        nsp << "JoinGameReply{"<< data.pdata << data.name << data.color << data.id << data.adr << data.port<<"}";
        return d;
    }
};


#endif // CLIENTDATA_HPP
