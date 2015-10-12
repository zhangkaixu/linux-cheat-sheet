#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, const char *argv[])
{
    const size_t BN = 1024;
    char buffer[BN];
    int N = 100;

    snprintf(buffer, BN, "mkdir -p %s", argv[1]);
    system(buffer);

    vector<FILE*> files;
    for (int i = 0; i < N; i++) {
        snprintf(buffer, BN, "%s/%d", argv[1], i);
        //printf("%s\n", buffer);
        files.push_back(fopen(buffer, "w"));
    }

    size_t ln = 0;
    for (string line; getline(cin, line); ) {
        ln++;
        if (ln % 10000 == 0) {
            fprintf(stderr, "%lu\r", ln / 10000);
        }
        size_t hash = 0;
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == '\t') break;
            hash = hash * 33 + line[i];
        }
        hash %= N;
        fprintf(files[hash], "%s\n", line.c_str());
    }

    for (int i = 0; i < N; i++) {
        fclose(files[i]);
    }


    return 0;
}
