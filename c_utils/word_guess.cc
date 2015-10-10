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
        _add_to_set(_dels, string(" ，。、！？：；“”《》"));

        _non_word_chars.insert(_dels.begin(), _dels.end());
        _add_to_set(_non_word_chars, string("0123456789,"));
    }

    void check(
            string& line,
            vector<unsigned int>& tokens,
            vector<size_t>& offs,
            vector<size_t>& begins, size_t len) {

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

        multimap<unsigned int, size_t> rights;

        size_t uniq = 0;
        for (size_t i = 0; i < begins.size(); i++) {
            unsigned int left = 0;
            unsigned int right = 0;
            if (begins[i] > 0) {
                left = tokens[begins[i] - 1];
            }
            if (begins[i] + len < tokens.size()) {
                right = tokens[begins[i] + len];
                rights.insert(pair<unsigned int, size_t>(
                            right, begins[i]));
            }

            bool context_ok = false;
            bool left_ok = false;
            bool right_ok = false;

            if (left_set.find(left) == left_set.end()) {
                left_ok = true;
                left_set.insert(left);
            } else {
                if (left == 0) {
                    left_ok = true;
                }
            }

            if (right_set.find(right) == right_set.end()) {
                right_ok = true;
                right_set.insert(right);
            } else {
                if (right == 0) {
                    right_ok = true;
                }
            }

            bigram_t key = (((bigram_t)left) << 32) | right;
            if (context.find(key) == context.end()) {
                context_ok = true;
                context.insert(key);
            } else {
                if (left == 0 || right == 0) {
                    context_ok = true;
                }
            }
            if (left_ok && right_ok && context_ok) {
                uniq++;
            }
        }

        if (uniq > 2) {
            string cand = line.substr(offs[begins[0]],
                    offs[begins[0] + len] - offs[begins[0]]);
            printf("%s\n", cand.c_str());
        }
        
        /// can expand?
        //
        vector<size_t> sub_begins;
        auto begin = rights.begin();
        while (true) {
            if (begin == rights.end()) break;

            size_t count = rights.count(begin->first);
            if (count == 1) {
                ++ begin;
                continue;};

            auto end = rights.upper_bound(begin->first);

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
        multimap<bigram_t, size_t> bigrams;

        /// 1 get bigrams
        for (size_t i = 0; i < tokens.size() - 1; i++) {
            bigrams.insert(pair<bigram_t, size_t>(
                        *((bigram_t*)&tokens[i]),
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
            auto end = bigrams.upper_bound(begin->first);

            /// repetitions
            begins.clear();
            for (auto iter = begin; iter != end; ++iter) {
                begins.push_back(iter->second);
            }
            check(line, tokens, offs, begins, 2);

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
