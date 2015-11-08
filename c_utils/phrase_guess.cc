#include <cstdio>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>

#include "utf8.h"
/**
 * 根据上下文的变化情况
 * 看一行汉字中有多少可能的词
 * */

using namespace std;

class CandFinder {
    typedef unsigned long long bigram_t;
    set<unsigned int> _dels;
    set<unsigned int> _non_word_chars;

    void _add_to_set(set<unsigned int>& s,
            string&& str) {
        vector<unsigned int> tokens;
        vector<size_t> offs;
        utf8::align_to_uint(str, tokens, offs); 
        for (size_t i = 0; i < tokens.size(); i++) {
            s.insert(tokens[i]);
        }
    }
public:

    CandFinder() {
        _add_to_set(_dels, string(" ,!?:;\"'，。、！？：；“”《》‘’…"));

        _non_word_chars.insert(_dels.begin(), _dels.end());
        //_add_to_set(_non_word_chars, string("0123456789,"));
    }

    void check(
            string& line,
            vector<unsigned int>& tokens,
            vector<size_t>& offs,
            vector<size_t>& begins, size_t len) {

        /// 排除包含某些字符的串
        if (_non_word_chars.find(tokens[begins[0]]) 
                != _non_word_chars.end()) {
            return;
        }
        if (_non_word_chars.find(tokens[begins[0] + len - 1]) != 
                _non_word_chars.end()) {
            return;
        }

        /// count
        set<bigram_t> context;
        set<unsigned int> left_set;
        set<unsigned int> right_set;

        vector<unsigned int> lefts;
        vector<unsigned int> rights;

        size_t uniq = 0;
        for (size_t i = 0; i < begins.size(); i++) {
            unsigned int left = 0;
            unsigned int right = 0;
            if (begins[i] > 0) {
                left = tokens[begins[i] - 1];
            } else {
                continue;
            }

            if (begins[i] + len < tokens.size()) {
                right = tokens[begins[i] + len];
            } else {
                continue;
            }

            bool ok = true;
            for (size_t j = 0; j < lefts.size(); j++) {
                if ((lefts[j] == left) 
                        && (_dels.find(left) == _dels.end())
                        ) {
                    ok = false;
                    break;
                }
                if ((rights[j] == right) 
                        && (_dels.find(right) == _dels.end())
                        ) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                lefts.push_back(left);
                rights.push_back(right);
            }
            if (lefts.size() > 1) break;

        }

        if (lefts.size() > 1) {
            string cand = line.substr(offs[begins[0]],
                    offs[begins[0] + len] - offs[begins[0]]);
            printf("%s\n", cand.c_str());
        }
        
        /// 扩展
        multimap<unsigned int, size_t> right_chars;
        for (size_t i = 0; i < begins.size(); i++) {
            unsigned int right = 0;
            if (begins[i] + len < tokens.size()) {
                right = tokens[begins[i] + len];
                right_chars.insert(pair<unsigned int, size_t>(
                            right, begins[i]));
            }
        }
        vector<size_t> sub_begins;
        auto begin = right_chars.begin();
        while (true) {
            if (begin == right_chars.end()) break;

            size_t count = right_chars.count(begin->first);
            if (count == 1) {
                ++ begin;
                continue;};

            auto end = right_chars.upper_bound(begin->first);

            /// repetitions
            sub_begins.clear();
            for (auto iter = begin; iter != end; ++iter) {
                sub_begins.push_back(iter->second);
            }
            check(line, tokens, offs, sub_begins, len + 1);
            begin = end;
        }
    }

    void find_repetition(
            string& line,
            vector<unsigned int>& tokens,
            vector<size_t>& offs) {
        multimap<unsigned int, size_t> bigrams;

        /// 1 get bigrams
        for (size_t i = 0; i < tokens.size() - 1; i++) {
            bigrams.insert(pair<unsigned int, size_t>(
                        *((unsigned int*)&tokens[i]),
                        i));
        }

        /// 2 inspect 
        vector<size_t> begins;
        auto begin = bigrams.begin();
        while (true) {
            if (begin == bigrams.end()) break;
            size_t count = bigrams.count(begin->first);
            if (count == 1) {
                ++ begin;
                continue;};

            /// 找到出现过不止一次的bigram
            
            auto end = bigrams.upper_bound(begin->first);

            /// 检查是否合格，并且对其进行向右的拓展
            begins.clear();
            for (auto iter = begin; iter != end; ++iter) {
                begins.push_back(iter->second);
            }
            check(line, tokens, offs, begins, 1);

            begin = end;
        }
    }
};


int main(int argc, const char *argv[])
{
    CandFinder finder;
    vector<unsigned int> tokens;
    vector<size_t> offs;
    size_t n = 0;
    for (string line; getline(cin, line); ) {
        utf8::align_to_uint(line, tokens, offs); 
        if (tokens.size() > 1000) {
            //printf("too long\n");
            tokens.resize(1000);
        }
        //cout<<line<<"\n";
        finder.find_repetition(line, tokens, offs);
        n++;
        if (n % 10000 == 0) {
            fprintf(stderr, "%10lu\r", n);
        }
    }
    
    return 0;
}
