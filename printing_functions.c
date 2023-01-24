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

void lastRowAlignment(wchar_t buffer[], wchar_t result[], int spaces_available, int spaces_per_hole, int additional_spaces, int is_last_row, int alignment){
    // different alignment if it's the last row
    if (is_last_row == 1){
        wchar_t s = L' ';
        int ind = 0;
        // LEFT
        if (alignment == 0){
            // concatenate whole buffer to the result
            if (buffer[ind] == L' ') ind++;
            while (buffer[ind] != L'\0'){
                wcsncat(result, &buffer[ind], 1);
                ind++;
            }
            // no need to add spaces, word have to be aligned to the left
        } // RIGHT
        else if (alignment == 1){
            for (int i = 0; i < (spaces_available); ++i) {
                wcsncat(result, &s, 1);
            }
            wcscat(result, buffer);
        } else if (alignment == 2){
            spaces_per_hole = spaces_available / 2;
            additional_spaces = spaces_available % 2;
            // left spaces
            for (int i = 0; i < (spaces_per_hole+additional_spaces); ++i) {
                wcsncat(result, &s, 1);
            }
            // word
            wcscat(result, buffer);
        }

    }
}

void additionalSpacesDistribution(wchar_t result[], int even, int additional_spaces, int words_counter, int N){
    int additional_len = wcslen(result);
    int index = additional_len-1;
    // align last according to the principals of a normal justification
    // in even rows
    if (even == 1) {
        while (additional_spaces > 0 && words_counter > 1) {
            // space was found
            if (isspace(result[index])) {
                int ind = index;
                while (isspace(result[ind])) {
                    ind--;
                }
                // shift right whole result to make place for additional space
                for (int c = N - 1; c > ind; c--) {
                    result[c] = result[c - 1];
                }
                // insert space
                result[ind + 1] = L' ';
                additional_spaces--;
            } else {
                index--;
            }
        }
    } else {
        // in odd words
        index = 0;
        while (additional_spaces > 0 && words_counter > 1) {
            // space was found
            if (isspace(result[index])) {
                int ind = index;
                while (isspace(result[ind])) {
                    ind++;
                }
                // shift right whole result to make place for additional space
                for (int c = N - 1; c >= ind; c--) {
                    result[c] = result[c - 1];
                }
                // insert space
                result[ind] = L' ';
                additional_spaces--;
            } else {
                index++;
            }
        }
    }


}

