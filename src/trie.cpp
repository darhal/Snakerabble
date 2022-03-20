#include "trie.hpp"
#include "Words.hpp"
#include <QtDebug>

Trie* Trie::s_Trie = nullptr;

Trie* Trie::get()
{
    static Trie* s_Trie = nullptr;
    if (s_Trie == nullptr) {
        s_Trie = new Trie();
        s_Trie->fill(ENGLISH_WORDS, sizeof(ENGLISH_WORDS)/sizeof(ENGLISH_WORDS[0]));
    }
    return s_Trie;
}

Trie::Trie()
{
    root = new TrieNode();
}

void Trie::fill(const char** arr, std::size_t len)
{
    TrieNode* cur = root;

    for (std::size_t i = 0; i < len; i++) {
        cur = root;

        for (size_t j = 0; arr[i][j]; j++) {
            char cl = arr[i][j];
            if (!cur->nodes[cl-'A']) {
                cur->nodes[cl-'A'] = new TrieNode();
            }
            cur = cur->nodes[cl-'A'];
        }

        cur->final = true;
    }
}

TrieNode* Trie::next(TrieNode* start, QChar c)
{
    if (!start)
        return start;
    char cl = c.toLatin1();
    return start->nodes[cl-'A'];
}
