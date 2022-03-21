#ifndef SNAKEDATA_HPP
#define SNAKEDATA_HPP

#include <QTimer>
#include <QPoint>
#include <qqml.h>
#include "snakepiecedata.hpp"
#include "trie.hpp"

struct SnakeData
{
    Q_GADGET
public:
    QVector<Point> positions;
    QVector<Point> wordRange;
    QString letters;
    TrieNode* trieNode;
    QString word;
    uint combo;
    uint score;
    Q_PROPERTY(QVector<Point> positions READ getPositions)
    Q_PROPERTY(QString letters READ getLetters)
    Q_PROPERTY(QString word MEMBER word)
    Q_PROPERTY(uint combo MEMBER combo)
    Q_PROPERTY(uint score MEMBER score)

    SnakeData() {
        combo = 0;
        score = 0;
        trieNode = Trie::get()->root;
        wordRange.emplaceBack(Point{0,0});
    }

    const QVector<Point>& getPositions() const
    {
        return positions;
    }

    const QString& getLetters() const
    {
        return letters;
    }

    void remove(uint start, uint end)
    {
        // qDebug() << "Remove between " << start << " e = " << end <<" - "<< positions.size()<<" - "<< letters.size();
        const auto pb = positions.begin();
        const auto pe = end >= positions.size() ? positions.end() : pb + end;
        positions.erase(pb + start, pe);
        const auto pbl = letters.begin();
        const auto pel = end >= letters.size() ? letters.end() : pbl + end;
        letters.erase(pbl + start, pel);
    }

    int eat(uint x, uint y, QChar l);

    void setData(const QVector<Point>& points, const QString& letters) {
        this->letters = letters;
        this->positions = points;
    }

    friend inline QDataStream& operator<<(QDataStream& s, const SnakeData& data)
    {
        s << data.getPositions() << data.getLetters();
        return s;
    }

    friend inline QDataStream& operator>>(QDataStream& s, SnakeData& data)
    {
        QVector<Point> positions;
        QString letters;
        s >> positions >> letters;
        data.setData(positions, letters);
        return s;
    }

    friend inline QDebug operator<<(QDebug d, const SnakeData& data){
        QDebug nsp = d.nospace();
        nsp << "positions="<<data.getPositions()<<",letters="<<data.getLetters();
        return d;
    }
};

Q_DECLARE_METATYPE(SnakeData)

class SnakeController : public QObject
{
    Q_OBJECT
    /*Q_PROPERTY(QVector<Point> positions READ getPositions NOTIFY dataChanged)
    Q_PROPERTY(QString letters READ getLetters NOTIFY dataChanged)*/
    Q_PROPERTY(SnakeData snakeData READ getSnakeData NOTIFY dataChanged)
public:
    void operator<<(const SnakePieceData& p)
    {
        snakeData.eat(p.x, p.y, p.letter);
    }

    QByteArray getBytes() const;

    const SnakeData& getSnakeData() const
    {
        return snakeData;
    }

    SnakeData& getSnakeData()
    {
        return snakeData;
    }

    void setData(const SnakeData& snakeData) {
        this->snakeData = snakeData;
        emit dataChanged();
    }

    Q_INVOKABLE void move(uint direction, uint gridWidth, uint gridHeight);

    Q_INVOKABLE void eat(uint x, uint y, QChar letter);

    Q_INVOKABLE void hit(uint x, uint y);

signals:
    void dataChanged();

    void death();

private:
    SnakeData snakeData;
};

inline QDataStream& operator<<(QDataStream& s, const SnakeController& data)
{
    s << data.getSnakeData();
    return s;
}

inline QDataStream& operator>>(QDataStream& s, SnakeController& data)
{
    SnakeData snakeData;
    s >> snakeData;
    data.setData(snakeData);
    return s;
}

#endif // SNAKEDATA_HPP
