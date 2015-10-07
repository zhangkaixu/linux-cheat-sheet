#pragma once
#include <cstdio>
#include <string>
#include <set>
#include <map>
#include <vector>

namespace utf8 {

using namespace std;

void align_to_uint(
        const string& input, 
        vector<unsigned int>& tokens,
        vector<size_t>& offs
        ) {
    unsigned int buffer = 0;
    tokens.clear();
    offs.clear();

    size_t i = 0;
    for (; i < input.size(); i++) {
        const unsigned char& c = input[i];
        if ((0xc0 == (c & 0xc0)) || !(c & 0x80)) { /// first char
            offs.push_back(i);
            if (buffer) {
                tokens.push_back(buffer);
                buffer = 0;
            }
        }
        buffer = (buffer << 8) | c;
    }
    offs.push_back(i);
    if (buffer) {
        tokens.push_back(buffer);
    }
};

}
