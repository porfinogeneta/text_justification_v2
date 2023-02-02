#include "headers.h"
#include "string.h"
#include "wctype.h"

void blowSpaces(wchar_t result[], wchar_t buffer[], int words_counter, int indent, int even, int N, int holes, int spaces_per_hole, int additional_spaces, int letters_counter){
    // NORMAL CASE JUSTIFICATION
    if (words_counter > 1){
        int ind = 0;
        while (ind < N+indent){
            // if we encountered word and didn't exceed word length
            while (( iswspace(buffer[ind])) == 0 && buffer[ind] != L'\0'){
                wcsncat(result, &buffer[ind], 1);
                ind++;
            }
            int placed_indent = 0;
            if (indent > 0 && ind == 0) {
                for (int i = 0; i < indent; ++i) {
                    wchar_t sp = L' ';
                    wcsncat(result, &sp, 1);
                }
                // avoid additional space
                placed_indent = 1;
                if (iswspace(buffer[indent])){
                    ind += indent;
                }else {
                    ind += indent-1;
                }
            }
            // if we encountered space, it's an obligatory one and it's not at the beginning
            if (iswspace(buffer[ind]) && holes > 0 && ind < N+indent && ind != 0 && placed_indent == 0){
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
    else if (words_counter == 1) {
        // it's a line that can accept some spaces
        if (letters_counter <= N){
            int ind = 0;
            wchar_t s = L'\u00A0';
            // add spaces on the left
            for (int i = 0; i < spaces_per_hole+indent; ++i) {
                wcsncat(result, &s, 1);
            }
            ind += indent;
            // add additional space on the left
            if (additional_spaces > 0 && even == 1){
                wcsncat(result, &s, 1);
            }
            // add word
            while (iswspace(buffer[ind]) == 0 && buffer[ind] != L'\0'){
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
    // ADDITIONAL SPACES
    additionalSpacesDistribution(result, even, additional_spaces, words_counter, N+indent, indent);
}

void lastRowAlignment(wchar_t buffer[], wchar_t result[], int N, int spaces_available, int is_last_row, int alignment){
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
        } // CENTER
        else if (alignment == 2){
            // calculate geometrical center of the string and line
            size_t string = wcslen(buffer)/2;
            int line_center = N / 2;
            int new_index = line_center - string;
            wchar_t sp = L' ';
            // adding spaces to get to the center
            for (int i = 0; i < new_index; ++i) {
                wcsncat(result, &sp, 1);
            }
            // adding word
            wcscat(result, buffer);
        }

    }
}

void additionalSpacesDistribution(wchar_t result[], int even, int additional_spaces, int words_counter, int N, int indent){
    size_t additional_len = wcslen(result);
    int index = additional_len-1;
    // in even rows
    if (even == 1) {
        while (additional_spaces > 0 && words_counter > 1) {
            // space was found
            if (iswspace(result[index])) {
                int ind = index;
                while (iswspace(result[ind])) {
                    ind--;
                }
                // shift right whole result to make place for additional space
                for (int c = N - 1; c > ind; c--) {
                    result[c] = result[c - 1];
                }
                // insert space
                result[ind + 1] = L' ';
                additional_spaces--;
                index = ind;
            } else {
                index--;
            }
        }
    } else {
        // in odd words
        index = indent;
        while (additional_spaces > 0 && words_counter > 1) {
            // space was found
            if (iswspace(result[index])) {
                int ind = index;
                while (iswspace(result[ind])) {
                    ind++;
                }
                // shift right whole result to make place for additional space
                for (int c = N - 1; c >= ind; c--) {
                    result[c] = result[c - 1];
                }
                // insert space
                result[ind] = L' ';
                additional_spaces--;
                index = ind;
            } else {
                index++;
            }
        }
    }
}

