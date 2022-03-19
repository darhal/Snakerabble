#ifndef SNAKEDATA_HPP
#define SNAKEDATA_HPP

#include <QTimer>
#include <QPoint>
#include <qqml.h>
#include "snakepiecedata.hpp"

class SnakeData : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVector<Point> positions READ getPositions NOTIFY dataChanged)
    Q_PROPERTY(QString letters READ getLetters NOTIFY dataChanged)
public:
    void operator<<(const SnakePieceData& p)
    {
        Point pt{p.x, p.y};
        positions << pt;
        letters.append(p.letter);
    }

    QByteArray getSnakeData() const;

    const QVector<Point>& getPositions() const
    {
        return positions;
    }

    const QString& getLetters() const {
        return letters;
    }

    void setData(const QVector<Point>& pos, const QString& letters) {
        this->positions = pos;
        this->letters = letters;
        emit dataChanged();
    }

    Q_INVOKABLE void move(uint direction, uint gridWidth, uint gridHeight);

    Q_INVOKABLE void eat(uint x, uint y, QChar letter);

signals:
    void dataChanged();

private:
    QVector<Point> positions;
    QString letters;
};

inline QDataStream& operator<<(QDataStream& s, const SnakeData& data)
{
    s << data.getPositions() << data.getLetters();
    return s;
}

inline QDataStream& operator>>(QDataStream& s, SnakeData& data)
{
    QVector<Point> positions;
    QString letters;
    s >> positions >> letters;
    data.setData(positions, letters);
    return s;
}

inline QDebug operator<<(QDebug d, const SnakeData& data){
    QDebug nsp = d.nospace();
    nsp << "positions="<<data.getPositions()<<",letters="<<data.getLetters();
    return d;
}


#endif // SNAKEDATA_HPP
