#ifndef TRIE_HPP
#define TRIE_HPP

#include <QObject>

struct TrieNode {
    TrieNode* nodes[26] = {};
    bool final;
};

class Trie
{
    static Trie* s_Trie;
public:
    static Trie* get();

    Trie();

    void fill(const char** arr, std::size_t len);

    TrieNode* next(TrieNode* start, QChar c);

public:
    TrieNode* root;
};

#endif // TRIE_HPP
