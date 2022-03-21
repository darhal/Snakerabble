#ifndef FOODSPAWNER_HPP
#define FOODSPAWNER_HPP

#include <QTimer>
#include <QString>
#include <QRandomGenerator>
#include "snakepiecedata.hpp"

class FoodSpawner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QHash<Point, uint> validPositions READ getValidPositions NOTIFY foodDataChanged)
    Q_PROPERTY(QVector<FoodData> foodData READ getFoodData NOTIFY foodDataChanged)
public:
    FoodSpawner();

    void init(uint n);

    void init(const QVector<FoodData>& fds);

    void spawn(uint idx, const FoodData& fd);

    void respawn(int idx);

    FoodData respawn(const FoodData& old, uint* fillIdx = nullptr);

    void destroy(Point pt);

    FoodData get(const Point& pt);

    const QHash<Point, uint>& getValidPositions() const { return validPositions; }

    const QVector<FoodData>& getFoodData() const { return foodData; }

    uint pickIndex();

    static uint random(int min, int max){
        return (QRandomGenerator::global()->generate64() + min) % max;
    }

signals:
    void foodDataChanged();

private:
    QHash<Point, uint> validPositions;
    QVector<FoodData>  foodData;

    static std::array<int, 26> ENGLISH_LETTER_FREQ;
    static std::array<int, 26> ENGLISH_LETTER_FREQ_PREFIX;
};

#endif // FOODSPAWNER_HPP
