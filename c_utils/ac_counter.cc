#include <cstdio>
#include <iostream>
#include <fstream>
#include "trie_automata.h"

using namespace std;
using namespace trie_automata;

/**
 * */

int main(int argc, const char** argv) {
    if (argc == 1) {
        printf("build AC-Automata using a dict file\n");
        printf("usage: %s dict_file \n", argv[0]);
        return 0;
    }
    TextDict text_dict;
    /// read text dict
    int index = 0;
    std::vector<std::string> keys;
    std::vector<size_t> counts;

    ifstream ifs(argv[1]);
    for (string line; getline(ifs, line); ) {
        size_t table_pos = line.find('\t');
        keys.push_back(line.substr(0, table_pos));
        counts.push_back(0);
        text_dict.AddKeyValue(keys.back(), index);
        index++;
    }

    fprintf(stderr, "there are %lu keys in the dict.\n", text_dict.size());
    std::shared_ptr<std::vector<ACNode>> trie = TrieBuilder<ACNode>().Build(text_dict);

    TrieAutomata<ACNode> ta(*trie);

    vector<int> matches;
    for (string line; getline(cin, line); ) {

        //printf("%s\n", line.c_str());
        unsigned int pos = 0;
        for (size_t i = 0; i < line.size(); i++) {
            unsigned char c = line[i];
            //printf("c : %lu\n", (unsigned char)c);
            pos = ta.search_next(pos, c);
            //printf("%lu\n", pos);
            ta.get_matches(pos, matches);
            //printf("%d\n", matches.size());
            for (size_t j = 0; j < matches.size(); j++) {
                counts[matches[j]]++;
            }
        }
        //printf("line end\n");
    }

    fprintf(stderr, "dump count\n");
    for (size_t i = 0; i < keys.size(); i++) {
        printf("%s\t%lu\n", keys[i].c_str(), counts[i]);
    }
    fprintf(stderr, "finished\n");

    return 0;
}
