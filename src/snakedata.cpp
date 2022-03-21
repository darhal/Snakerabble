#include "snakedata.hpp"


int SnakeData::eat(uint x, uint y, QChar l)
{
    Point nPos = {x, y};
    positions.push_front(nPos);
    letters.push_front(l);
    TrieNode* nextTrieNode = Trie::get()->next(trieNode, l);
    word.append(l);

    if (nextTrieNode == nullptr) {
        trieNode = Trie::get()->root;
        word.clear();
        combo = 0;
        if (wordRange.back().x == wordRange.back().y)
            wordRange.pop_back();
        wordRange.emplaceBack(Point{(uint)positions.size() - 1, (uint)positions.size() - 1});
        qDebug() << wordRange;
        qDebug() << "no words";
        return 2;
    }else if (nextTrieNode->final) {
        ++combo;
        score += word.size() * combo;
        wordRange.back().y = positions.size() - 1;
        //wordRange.emplaceBack(Point{(uint)positions.size() - 1, (uint)positions.size() - 1});
        trieNode = nextTrieNode;
        qDebug() << "Created a word " << word << " combo = " << combo;
        qDebug() << wordRange;
        return 1;
    }else{
        trieNode = nextTrieNode;
    }

    return 0;
}

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

    this->hit(head.x, head.y);
    snakeData.positions.push_front(head);
    emit dataChanged();
}

void SnakeController::eat(uint x, uint y, QChar letter)
{
    snakeData.eat(x, y, letter);
    // emit dataChanged();
}

void SnakeController::hit(uint x, uint y)
{
    Point pos = {x, y};
    auto itr = std::find_if(snakeData.positions.begin(), snakeData.positions.end(), [&pos](const auto& p) { return p == pos; });

    if (itr != snakeData.positions.end()) {
        auto hitIdx = std::distance(itr, snakeData.positions.end());
        auto lb = std::lower_bound(snakeData.wordRange.begin(), snakeData.wordRange.end(), hitIdx,
                                   [](const Point& r, uint l){return r.x < l;});
        if (lb == snakeData.wordRange.end())
            qDebug() << "out of bound Remove " << lb->x << "," << lb->y;
        if (lb != snakeData.wordRange.end() && lb->x != lb->y && lb->x <snakeData.positions.size()) {
            qDebug() << "Remove " << lb->x << "," << lb->y;
            const auto pb = snakeData.positions.begin();
            const auto pe = lb->y >= snakeData.positions.size() ? snakeData.positions.end() : pb + lb->y;
            snakeData.positions.erase(pb + lb->x, pe);
            const auto pbl = snakeData.letters.begin();
            const auto pel = lb->y >= snakeData.letters.size() ? snakeData.letters.end() : pbl + lb->y;
            snakeData.letters.erase(pbl + lb->x, pel);
            auto diff = lb->y - lb->x + 1;
            qDebug() << "diff " << diff;
            for (auto lbitr = ++lb; lbitr != snakeData.wordRange.end(); lbitr++) {
                lbitr->x -= diff;
                lbitr->y -= diff;
                qDebug() << *lbitr;
            }
            snakeData.wordRange.erase(lb);
            qDebug() << snakeData.wordRange;
        }else{
            qDebug() << "Die!";
        }
    }
}

/*
[1,3],[5,7]

QList(Point{0,4}, Point{5,7}, Point{8,10}, Point{11,13})
*/
