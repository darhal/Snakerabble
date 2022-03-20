#include "snakedata.hpp"


QByteArray SnakeController::getBytes() const
{
    QByteArray bytes;
    QDataStream ds(&bytes, QIODeviceBase::WriteOnly);
    ds << this->snakeData;
    return bytes;
}

void SnakeController::move(uint direction, uint gridWidth, uint gridHeight)
{
    auto head = snakeData.positions.first();
    snakeData.positions.pop_back();

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

    snakeData.positions.push_front(head);
    emit dataChanged();
}

void SnakeController::eat(uint x, uint y, QChar letter)
{
    snakeData.eat(x, y, letter);
    // emit dataChanged();
}
