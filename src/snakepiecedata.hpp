#ifndef SNAKEPIECEDATA_H
#define SNAKEPIECEDATA_H

#include <QTimer>
#include <QDataStream>
#include <QDebug>



struct SnakePieceData
{
    Q_GADGET
public:
    uint x, y;
    QChar letter;
    Q_PROPERTY(uint x MEMBER x)
    Q_PROPERTY(uint y MEMBER y)
    Q_PROPERTY(QChar letter MEMBER letter)
};
Q_DECLARE_METATYPE(SnakePieceData)
using FoodData = SnakePieceData;

inline QDataStream& operator<<(QDataStream& s, const SnakePieceData& data)
{
    s << data.x << data.y << data.letter;
    return s;
}

inline QDataStream& operator>>(QDataStream& s, SnakePieceData& data)
{
    s >> data.x >> data.y >> data.letter;
    return s;
}

inline QDebug operator<<(QDebug d, const SnakePieceData& data){
    QDebug nsp = d.nospace();
    nsp << "SnakePieceData{" <<data.x << "," << data.y << "," << data.letter << "}";
    return d;
}

struct Point
{
    Q_GADGET
public:
    uint x, y;
    Q_PROPERTY(uint x MEMBER x)
    Q_PROPERTY(uint y MEMBER y)

    bool operator==(const Point& other) const { return (x == other.x && y == other.y); }
};
Q_DECLARE_METATYPE(Point)

inline QDataStream& operator<<(QDataStream& s, const Point& data)
{
    s << data.x << data.y;
    return s;
}

inline QDataStream& operator>>(QDataStream& s, Point& data)
{
    s >> data.x >> data.y;
    return s;
}

inline QDebug operator<<(QDebug d, const Point& data){
    QDebug nsp = d.nospace();
    nsp << "Point{" <<data.x << "," << data.y << "}";
    return d;
}

namespace std {
  template <>
  struct hash<Point>
  {
    std::size_t operator()(const Point& k) const
    {
      return (std::size_t)k.x << 32 | (std::size_t)k.y;
    }
  };
}

#endif // SNAKEPIECEDATA_H
