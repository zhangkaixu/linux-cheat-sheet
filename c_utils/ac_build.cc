#include <cstdio>
#include <iostream>
#include "trie_automata.h"

using namespace std;
using namespace trie_automata;

/**
 * ##ac_build
 * 用于创建AC自动机
 * */

int main(int argc, const char** argv) {
    if (argc == 1) {
        printf("build AC-Automata using a dict file\n");
        printf("usage: %s ac.bin < ordered_dict_file\n", argv[0]);
        return 0;
    }
    TextDict text_dict;
    /// read text dict
    for (string line; getline(cin, line); ) {
        text_dict.AddLine(line);
    }
    printf("%lu\n", text_dict.size());

    /// build trie
    std::shared_ptr<std::vector<ACNode>> trie = TrieBuilder<ACNode>().Build(text_dict);
    FILE* pf = fopen("test.trie", "wb");
    save_trie(*trie, pf);
    return 0;
}
