#include <cstdio>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>

#include "counter.h"

using namespace std;

int main(int argc, const char *argv[])
{
    counter::Counter<string> counter;
    const unsigned char sep = '\t';

    size_t ms = -1;
    if (argc > 1) {
        ms = atoi(argv[1]);
    }

    size_t c = 0;
    size_t n = 0;
    for (string line; getline(cin, line); ) {
        if (c >= ms) {
            fprintf(stderr, "[%lu]\r", n);
            counter.dump();
            counter.clear();
            c = 0;
        }
        size_t pos = line.rfind(sep);
        if (pos == string::npos) {
            counter.update(line);
        } else {
            long count = atoi(&line[pos + 1]);
            counter.update(line.substr(0, pos), count);
        }
        c += 1;
        n += 1;
    }
    counter.dump();

    return 0;
}
