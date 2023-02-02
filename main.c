#include "stdio.h"
#include "stdlib.h"
#include "wchar.h"
#include "ctype.h"
#include "wctype.h"
#include "headers.h"
#include "string.h"
#include <locale.h>


int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    fwide(stdin, 1);

    char LAST_TYPES[4][7] = {"left", "right", "center", "justify"};

    // SETTINGS
    int N = 72;
    int LAST = 2;
    int INDENT = 0;


    wchar_t *readingBuffer = calloc(N*3, sizeof(wchar_t));
    wchar_t *help_buff = calloc(N*3, sizeof(wchar_t));
//    wchar_t help_buff[N*3];

//    wmemset(readingBuffer, L'\0', N*3);
//    wmemset(help_buff, L'\0', N*3);

    for (int i = 1; i < argc; i++)
    {
        if (argc % 2 == 0)
        {
            fprintf(stderr, "Not enough arguments");
            return 1;
        }

        if (strcmp("--last", argv[i]) == 0)
        {
            i++;
            for (int j = 0; j < 4; j++)
            {
                if (strcmp(LAST_TYPES[j], argv[i]) == 0)
                {
                    LAST = j;
                    break;
                }
            }
            continue;
        }
        if (strcmp("--width", argv[i]) == 0)
        {
            i++;
            N = atoi(argv[i]);
            continue;
        }
        if (strcmp("--indent", argv[i]) == 0)
        {
            i++;
            INDENT = atoi(argv[i]);
            continue;
        }

    }

    wint_t c;
//    wchar_t nl = L'\n';
//    wcsncat(readingBuffer, &nl, 1);
    wchar_t indt = L' ';
    for (int i = 0; i < INDENT; ++i) {
        wcsncat(readingBuffer, &indt, 1);
    }
    int letters = INDENT;
    int br = 0;
    int whites = 0;


    while  ((c = fgetwc(stdin)) != WEOF)  {
        // LETTER
        if (iswspace(c) == 0){
            // before letter was one br
            if (br == 1){
                wchar_t w = L' ';
                wcsncat(readingBuffer, &w, 1);
            }
            if (br >= 2){
                // adding double breakline
                wchar_t w = L'\n';
                wcsncat(readingBuffer, &w, 1);
                wcsncat(readingBuffer, &w, 1);
                // run program
//                *paragraph_started = 1;
                readingBuffer = justify_center(readingBuffer, help_buff, N, LAST, INDENT);
                input_place = 1;
//                int id = 0;
//                while (readingBuffer[id] != '\0'){
//                    printf("%lc", *readingBuffer);
//                    id++;
//                }
//                *paragraph_started = 0;
                // RESET
                // set indent
                for (int i = 0; i < INDENT; ++i) {
                    wcsncat(readingBuffer, &indt, 1);
                }
                letters = INDENT;
                br = 0;
                whites = 0;
            }

            wchar_t w = c;
            wcsncat(readingBuffer, &w, 1);
            letters++;
            // restart spaces/br counter if we stepped on a word
            whites = 0;
            br = 0;
        }
        if (iswspace(c)){
            if (letters >= N + 1){
                // run functions
                readingBuffer = justify_center(readingBuffer, help_buff, N, LAST, INDENT);
                input_place = 1;
//                int id = 0;
//                while (readingBuffer[id] != '\0'){
//                    printf("%lc", *readingBuffer);
//                    id++;
//                }
//                printf("%ls", readingBuffer);
                letters = wcslen(readingBuffer);
                br = 0;
                whites = 0;
            }
            whites++;
            if (c == L'\n'){
                br++;
            }
            else {
                if (whites < 2){
                    wchar_t w = L' ';
                    wcsncat(readingBuffer, &w, 1);
                }
            }
        }
    }
}