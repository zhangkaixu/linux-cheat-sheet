#include <cstdio>
#include <vector>
#include <iostream>
#include "trie_automata.h"


using namespace std;
using namespace trie_automata;

int main(int argc, const char** argv) {
    vector<ACNode> trie;
    load_trie(trie, fopen("test.trie", "rb"));

    TrieAutomata<ACNode> ta(trie);


    vector<int> matches;
    for (string line; getline(cin, line); ) {
        unsigned int pos = 0;
        cout<<line;
        for (size_t i = 0; i < line.size(); i++) {
            unsigned char c = line[i];
            pos = ta.search_next(pos, c);
            ta.get_matches(pos, matches);
            for (size_t j = 0; j < matches.size(); j++) {
                cout<<" "<<line.substr(i + 1 - matches[j], matches[j]);
            }
        }
        cout<<"\n";
    }

    return 0;
}
