#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char *argv[])
{
    FILE* pf = fopen(argv[1], "r");

    char* line = NULL;
    size_t line_size = 0;
    size_t size = 0;
    size_t foffset = ftell(pf);
    while ((size = getline(&line, &line_size, pf)) != -1) {
        char* pt = strchr(line, '\t');
        if (pt == NULL) continue;
        *pt = '\0';
        printf("%s\t%lu\n", line, foffset + (pt - line + 1));
        foffset = ftell(pf);
    }


    fclose(pf);

    if (line) free(line);
    
    return 0;
}
