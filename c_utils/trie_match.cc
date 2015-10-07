#include <cstdio>
#include <iostream>
#include "trie_automata.h"


using namespace std;
using namespace trie_automata;

int main(int argc, const char** argv) {
    vector<TrieNode> trie;
    load_trie(trie, fopen("test.trie", "rb"));

    TrieAutomata<TrieNode> ta(trie);

    for (string line; getline(cin, line); ) {
        printf("%d\n", ta.match(line.c_str()));
    }

    return 0;
}
