#include "foodspawner.hpp"
#include <random>
#include <QDateTime>
#include "server.hpp"
#include <QRandomGenerator>

std::array<int, 26> FoodSpawner::ENGLISH_LETTER_FREQ = {
    817, 149, 278, 425, 1270, 223, 202, 609, 697, 15, 77, 403,
    241, 675, 751, 193, 9, 599, 633, 906, 276, 97, 236, 15, 197, 7
};
std::array<int, 26> FoodSpawner::ENGLISH_LETTER_FREQ_PREFIX = {};

uint random(int min, int max){
    return (QRandomGenerator::global()->generate64() + min) % max;
}

FoodSpawner::FoodSpawner()
{
    for (int i = 0; i < 26; i++) {
        auto prev = i == 0 ? 0 : ENGLISH_LETTER_FREQ_PREFIX[i-1];
        ENGLISH_LETTER_FREQ_PREFIX[i] = prev + ENGLISH_LETTER_FREQ[i];
    }
}

uint FoodSpawner::pickIndex()
{
    uint target = random(0, ENGLISH_LETTER_FREQ_PREFIX[25]);
    auto upb = std::upper_bound(ENGLISH_LETTER_FREQ_PREFIX.begin(), ENGLISH_LETTER_FREQ_PREFIX.end(), target);
    return std::distance(ENGLISH_LETTER_FREQ_PREFIX.begin(), upb);
}

void FoodSpawner::init(uint n)
{
    auto gridWidth = Server::getServer()->getGridSize().x;
    auto gridHeight = Server::getServer()->getGridSize().y;

    for (uint i = 0; i < n; i++) {
        Point pt{random(0, gridWidth), random(0, gridHeight)};
        foodData.emplaceBack(FoodData{pt.x, pt.y, QChar(this->pickIndex()+'A')});
        validPositions.insert(pt, i);
    }

    emit foodDataChanged();
}

void FoodSpawner::init(const QVector<FoodData>& fds)
{
    foodData = fds;
    uint i = 0;

    for (const auto& fd : fds) {
        Point pt{fd.x, fd.y};
        validPositions.insert(pt, i++);
    }

    emit foodDataChanged();
}

void FoodSpawner::spawn(uint idx, const FoodData& fd)
{
    foodData[idx] = fd;
    validPositions.insert({fd.x, fd.y}, idx);
    emit foodDataChanged();
}

void FoodSpawner::respawn(int idx)
{
    this->respawn(foodData[idx]);
}

FoodData FoodSpawner::respawn(const FoodData& old, uint* fillIdx)
{
    auto itr = validPositions.find({old.x, old.y});
    FoodData res{0, 0, QChar(0)};

    if (itr != validPositions.end()) {
        Point pt;
        auto gridWidth = Server::getServer()->getGridSize().x;
        auto gridHeight = Server::getServer()->getGridSize().y;
        auto idx = itr.value();
        if (fillIdx) *fillIdx = idx;
        validPositions.erase(itr);
        pt.x = random(0, gridWidth);
        pt.y = random(0, gridHeight);
        this->spawn(idx, FoodData{pt.x, pt.y, QChar(random(0,26)+'A')});
        res.x = pt.x; res.y = pt.y; res.letter = foodData[idx].letter;
    }

    return res;
}

void FoodSpawner::destroy(Point pt)
{
    validPositions.remove(pt);
}

FoodData FoodSpawner::get(const Point& pt)
{
    auto itr = validPositions.find(pt);
    FoodData res{0, 0, QChar(0)};

    if (itr != validPositions.end() && itr.value() < foodData.size()) {
        auto idx = itr.value();
        if (idx < foodData.size()) {
            res = foodData[idx];
        }
    }

    return res;
}
