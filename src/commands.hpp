#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QObject>
#include <QDataStream>
#include <QDebug>
#include "snakedata.hpp"
#include "clientdata.hpp"

struct Command
{
    enum {
        JOIN_GAME,
        JOIN_GAME_REPLY,
        BROADCAST_SYNC,
        SEND_PLAYER_DATA
    };

    uint id;
    QByteArray cmdData;

    Command(uint id, const QByteArray& cmd) : id(id), cmdData(cmd)
    {
    }

    Command(QByteArray& bytes)
    {
        QDataStream ds(&bytes, QIODeviceBase::ReadOnly);
        ds >> this->id >> this->cmdData;
    }

    template<typename T>
    Command(const T& cmd)
    {
        this->id = T::ID;
        QDataStream ds(&cmdData, QIODeviceBase::WriteOnly);
        ds << cmd;
    }

    template<typename T>
    T getData()
    {
        if (T::ID != this->id)
            return T{};
        QDataStream ds(&cmdData, QIODeviceBase::ReadOnly);
        T cmd;
        ds >> cmd;
        return cmd;
    }

    QByteArray getBytes()
    {
        QByteArray bytes;
        QDataStream ds(&bytes, QIODeviceBase::WriteOnly);
        ds << this->id << this->cmdData;
        return bytes;
    }
};

// Client to Server
struct JoinGameCmd
{
    using Cmd = JoinGameCmd;
    constexpr static uint ID = Command::JOIN_GAME;
    QString name;
    SnakeData data;
    uint color;

    friend inline QDataStream& operator<<(QDataStream& s, const Cmd& data)
    {
        s << data.name << data.data << data.color;
        return s;
    }

    friend inline QDataStream& operator>>(QDataStream& s, Cmd& data)
    {
        s >> data.name >> data.data >> data.color;
        return s;
    }

    friend inline QDebug operator<<(QDebug d, const Cmd& data){
        QDebug nsp = d.nospace();
        nsp << "JoinGameCmd{"<<data.name<<","<<data.data<<","<<data.color<<"}";
        return d;
    }
};

struct JoinGameReply
{
    using Cmd = JoinGameReply;
    constexpr static uint ID = Command::JOIN_GAME_REPLY;
    QVector<ClientData> playersData;
    uint id;

    friend inline QDataStream& operator<<(QDataStream& s, const Cmd& data)
    {
        s << data.playersData << data.id;
        return s;
    }

    friend inline QDataStream& operator>>(QDataStream& s, Cmd& data)
    {
        s >> data.playersData >> data.id;
        return s;
    }

    friend inline QDebug operator<<(QDebug d, const Cmd& data){
        QDebug nsp = d.nospace();
        nsp << "JoinGameReply{"<<data.playersData<<","<<data.id<<"}";
        return d;
    }
};

// Server to ALL
struct JoinBroadcastCmd
{
    using Cmd = JoinBroadcastCmd;
    constexpr static uint ID = Command::JOIN_GAME;
    ClientData data;

    friend inline QDataStream& operator<<(QDataStream& s, const Cmd& data)
    {
        s << data.data;
        return s;
    }

    friend inline QDataStream& operator>>(QDataStream& s, Cmd& data)
    {
        s >> data.data;
        return s;
    }

    friend inline QDebug operator<<(QDebug d, const Cmd& data){
        QDebug nsp = d.nospace();
        nsp << "JoinBroadcastCmd{"<<data.data<<"}";
        return d;
    }
};

// Server to ALL
struct SyncGameCmd
{
    using Cmd = SyncGameCmd;
    constexpr static uint ID = Command::BROADCAST_SYNC;
    QVector<ClientData> playersData;

    friend inline QDataStream& operator<<(QDataStream& s, const Cmd& data)
    {
        s << data.playersData;
        return s;
    }

    friend inline QDataStream& operator>>(QDataStream& s, Cmd& data)
    {
        s >> data.playersData;
        return s;
    }

    friend inline QDebug operator<<(QDebug d, const Cmd& data){
        QDebug nsp = d.nospace();
        nsp << "SyncGameCmd{"<<data.playersData<<"}";
        return d;
    }
};

// Client to Server
struct SendPlayerCmd
{
    using Cmd = SendPlayerCmd;
    constexpr static uint ID = Command::SEND_PLAYER_DATA;
    SnakeData playerData;
    uint id;

    friend inline QDataStream& operator<<(QDataStream& s, const Cmd& data)
    {
        s << data.playerData << data.id;
        return s;
    }

    friend inline QDataStream& operator>>(QDataStream& s, Cmd& data)
    {
        s >> data.playerData >> data.id;
        return s;
    }

    friend inline QDebug operator<<(QDebug d, const Cmd& data){
        QDebug nsp = d.nospace();
        nsp << "SyncGameCmd{"<<data.playerData<<","<<data.id<<"}";
        return d;
    }
};

#endif // COMMANDS_HPP
