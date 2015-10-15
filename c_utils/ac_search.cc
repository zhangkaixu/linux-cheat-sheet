#include <cstdio>
#include <vector>
#include <iostream>
#include "trie_automata.h"


using namespace std;
using namespace trie_automata;

int main(int argc, const char** argv) {
    if (argc == 1) {
        printf("usage: %s ac.bin < input > output\n", argv[0]);
        return 0;
    }
    vector<ACNode> trie;
    load_trie(trie, fopen(argv[1], "rb"));

    TrieAutomata<ACNode> ta(trie);


    vector<int> matches;
    for (string line; getline(cin, line); ) {
        unsigned int pos = 0;
        cout<<line;
        for (size_t i = 0; i < line.size(); i++) {
            unsigned char c = line[i];
            pos = ta.search_next(pos, c);
            ta.get_matches(pos, matches);
            //printf(" %lu", matches.size());
            for (size_t j = 0; j < matches.size(); j++) {
                cout<<" "<<line.substr(i + 1 - matches[j], matches[j]);
            }
        }
        cout<<"\n";
    }

    return 0;
}
