#include "snakedata.hpp"


QByteArray SnakeData::getSnakeData() const
{
    QByteArray bytes;
    QDataStream ds(&bytes, QIODeviceBase::WriteOnly);
    ds << this->getPositions() << this->getLetters();
    return bytes;
}


void SnakeData::move(uint direction, uint gridWidth, uint gridHeight)
{
    auto head = positions.first();
    positions.pop_back();

    auto mod = [](int x, int n) {
        return ((x % n) + n) % n;
    };

    switch (direction) {
    case 0:
        head.y = mod(head.y - 1, gridHeight);
        break;
    case 1:
        head.y = mod(head.y + 1, gridHeight);
        break;
    case 2:
        head.x = mod(head.x - 1, gridWidth);
        break;
    case 3:
        head.x = mod(head.x + 1, gridWidth);
        break;
    }

    positions.push_front(head);
    emit dataChanged();
}

void SnakeData::eat(uint x, uint y, QChar letter)
{
    positions.push_front({x, y});
    letters.push_front(letter);
    emit dataChanged();
}
