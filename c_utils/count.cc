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

    for (string line; getline(cin, line); ) {
        size_t pos = line.rfind(sep);
        if (pos == string::npos) {
            counter.update(line);
        } else {
            long count = atoi(&line[pos + 1]);
            counter.update(line.substr(0, pos), count);
        }
    }
    counter.dump();

    return 0;
}
