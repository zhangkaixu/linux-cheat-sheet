#include <cstdio>
#include <iostream>
#include "trie_automata.h"

using namespace std;
using namespace trie_automata;

int main(int argc, const char** argv) {
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
