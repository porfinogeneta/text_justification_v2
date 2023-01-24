#include "headers.h"
#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include "stdlib.h"

void blowSpaces(wchar_t result[], wchar_t buffer[], int words_counter, int ident, int even, int is_last_row, int alignment, int N, int holes, int spaces_per_hole, int additional_spaces, int letters_counter){
    // NORMAL CASE JUSTIFICATION
    if (words_counter > 1 && ( is_last_row == 0 || alignment == 3)){
        int ind = 0;
        while (ind < N){
            // if we encountered word and didn't exceed word length
            while (( isspace(buffer[ind])) == 0 && buffer[ind] != L'\0'){
                wcsncat(result, &buffer[ind], 1);
                ind++;
            }
//            ident = 0;
            // if we encountered space and it's an obligatory one and it's not at the beginning
            if (isspace(buffer[ind]) && holes > 0 && ind < N && ind != 0){
                wchar_t w = L' ';
                for (int i = 0; i < spaces_per_hole; ++i) {
                    wcsncat(result, &w, 1);
                }
                holes--;
                wcsncat(result, &w, 1);
            }

            // distributing additional_spaces
            ind++;
        }
    }
        // ONE WORD FITTING CASE
    else if (words_counter == 1 && (is_last_row == 0 || alignment == 3)) {
        // it's a line that can accept some spaces
        if (letters_counter <= N){
            int ind = 0;
            wchar_t s = L'\u00A0';
            // add spaces on the left
            for (int i = 0; i < spaces_per_hole; ++i) {
                wcsncat(result, &s, 1);
            }
            // add additional space on the left
            if (additional_spaces > 0 && even == 1){
                wcsncat(result, &s, 1);
            }
            // add word
            while (isspace(buffer[ind]) == 0 && buffer[ind] != L'\0'){
                wcsncat(result, &buffer[ind], 1);
                ind++;
            }
            // add spaces on the right
            for (int i = 0; i < spaces_per_hole; ++i) {
                wcsncat(result, &s, 1);
            }
            // add space on the left
            if (additional_spaces > 0 && even == -1){
                wcsncat(result, &s, 1);
            }
        }

    }
}

