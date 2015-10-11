#include <cstdio>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <stack>

#include "utf8.h"
#include "trie_automata.h"
/**
 * 根据一本词典，搜索文本中所有的交集歧义片段
 * */

using namespace std;
using namespace trie_automata;


struct span_t {
    size_t begin;
    size_t end;
    span_t(size_t b, size_t e):begin(b), end(e){};
};


class OverlappingFinder {
private:
    set<pair<size_t, size_t>> _span_set;
    vector<vector<size_t>> _next;
    struct search_state_t {
        int span_index;
        int inner_mark;
        int end;
        search_state_t(int ind, int mark, int e):
            span_index(ind), inner_mark(mark), end(e){};
    };
public:
    void find(string&line, vector<span_t>& spans,
            vector<span_t>& overlappings) {

        /// Step 1 : sort spans
        /// sorted
        /// Step 2 : find unit overlapping
        while (_next.size() < spans.size()) {
            _next.push_back(vector<size_t>());
        }
        for (int i = 0; i < spans.size(); i++) {
            _next[i].clear();
        }

        for (int i = spans.size() - 1; i >= 0; i--) {
            //printf("%d\n", i);
            //cout<<line.substr(spans[i].begin, spans[i].end - spans[i].begin)<<"\n";
            for (int j = i - 1; j >= 0; j--) {
                if (spans[j].end == spans[i].end) continue;
                if (spans[j].end <= spans[i].begin) break;

                if (spans[j].begin < spans[i].begin) {
                    _next[i].push_back(j);
                }
                
            }
        }

        /// Step 3 : expand
        _span_set.clear();
        stack<search_state_t> s;
        for (int i = spans.size() - 1; i >= 0; i--) {
            vector<size_t>& nexts = _next[i];
            for (auto iter = nexts.begin(); iter!= nexts.end(); ++iter) {
                int j = *iter;
                //cout<<line.substr(spans[j].begin, spans[j].end - spans[j].begin)<<" ";
                //cout<<line.substr(spans[i].begin, spans[i].end - spans[i].begin)<<"\n";
                while (s.size()) {s.pop();};
                s.push(search_state_t(j, spans[i].begin, spans[i].end));
                while (s.size()) {
                    search_state_t top = s.top();
                    s.pop();
                    size_t begin = spans[top.span_index].begin;
                    size_t end = top.end;
                    _span_set.insert(pair<size_t, size_t>(begin, end));
                    //cout << line.substr(begin, end - begin) <<"\n";
                    //cout<< "span[j="<<j<<"]" << line.substr(spans[j].begin, spans[j].end - spans[j].begin)<<"\n";
                    vector<size_t>& ne = _next[top.span_index];
                    for (auto it = ne.begin(); it != ne.end(); ++it) {
                        int k = *it;
                        if (spans[k].end > top.inner_mark) continue;
                        //cout <<"very \n";
                        //cout<< "span[k="<<k<<"]" << line.substr(spans[k].begin, spans[k].end - spans[k].begin)<<"\n";
                        s.push(search_state_t(k, spans[j].begin, top.end));
                    }
                }
            }
        }

        for (auto iter = _span_set.begin(); iter != _span_set.end(); ++iter) {
            cout << line.substr(iter->first, iter->second - iter->first) <<"\n";
        }

    }
};

int main(int argc, const char *argv[])
{
    if (argc == 1) {
        printf("usage: %s ac.bin < input > output\n", argv[0]);
        return 0;
    }

    /// load AC
    vector<ACNode> trie;
    load_trie(trie, fopen(argv[1], "rb"));
    TrieAutomata<ACNode> ac(trie);

    OverlappingFinder finder;

    vector<unsigned int> tokens; ///< 字符数组
    vector<size_t> offs;
    size_t n = 0;

    vector<span_t> spans;
    vector<span_t> overlappings;

    for (string line; getline(cin, line); ) {
        utf8::align_to_uint(line, tokens, offs); 
        //cout<<line<<"\n";

        /// get all spans
        spans.clear();
        unsigned int pos = 0;
        vector<int> matches;
        for (size_t i = 0; i < tokens.size(); i++) {
            for (size_t j = offs[i]; j < offs[i+1]; j++) {
                unsigned char c = line[j];
                pos = ac.search_next(pos, c);
            }
            ac.get_matches(pos, matches); /// get for every character
            for (size_t j = 0; j < matches.size(); j++) {
                spans.push_back(span_t(offs[i + 1] - matches[j], offs[i + 1]));
            }
        }

        /// find overlapping
        finder.find(line, spans, overlappings);
    }
    
    return 0;
}
