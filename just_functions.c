#include "headers.h"
#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include "stdlib.h"

void prepare_text(wchar_t input[], int N, int *finishedPreparing, wchar_t *buffer) {

    int letters = 0;
    int index = 0;
    wchar_t *preparedBuffer = calloc(N*3, sizeof(wchar_t));
    while (letters < N+1){
        int ind = index;
        // LETTER
        if (isspace(input[index]) == 0){
            while (isspace(input[ind]) == 0) {
                wchar_t w = input[ind];
                wcsncat(preparedBuffer, &w, 1);
                ind++;
                letters++;
;            }
        } // WHITE SIGN
        else {
            // NEW PARAGRAPH
            if (input[ind] == '\n'){
                while (input[ind] == '\n') {
                    ind++;
                }
                // new paragraph
                if ((ind - index) >= 2){
                    wchar_t br[3] = L"\n\n\0";
                    wcscat(preparedBuffer, br);
                    size_t pBL = wcslen(preparedBuffer);
                    preparedBuffer[pBL] = '\0';
                    *finishedPreparing = ind;
                    wcscat(buffer, preparedBuffer);
                    return;
                }// space instead of paragraph
                else {
                    wchar_t sp = L' ';
                    wcsncat(preparedBuffer, &sp, 1);
                }
            } // SPACE
            else if (input[ind] == L' '){
                while (input[ind] == L' ') {
                    ind++;
                }
                wchar_t sp = L' ';
                wcsncat(preparedBuffer, &sp, 1);
            } else {
                while (isspace(input[ind])) {
                    ind++;
                }
            }
        }
        index = ind;
    }
    *finishedPreparing = index;

    size_t pBL = wcslen(preparedBuffer);
    preparedBuffer[pBL] = '\0';
    wcscat(buffer, preparedBuffer);
//    printf("%s\n", bufferNewLines);
    memset(preparedBuffer, L'\0', N*3);
}


void justify_center(wchar_t buffer[], wchar_t input[], int N, int alignment, int ident){
    // pointer to the index where we finished preparing our text
    int finishedPreparing = 0;
    int paragraph_counter = 0;
    int is_end_paragraph = 0;
    wchar_t help_buff[N*3];
    int even = -1; // is even 1, is not -1
    // clear buffer additional before everything
    for (int j = 0; j < (3 * N) - 1; ++j) {
        help_buff[j] = L'\0';
    }
    size_t i = 0;
    while (i < wcslen(input) || help_buff[0] != L'\0') {

        // prepare our text to justification
        // copy text not justified text into our buffer to handle it
        size_t help_buff_len = wcslen(help_buff);
        // check if the paragraph is coming to the end
        if (help_buff[help_buff_len-1] == L'\n'){
            is_end_paragraph = 1;
            paragraph_counter++;
        }else {
            is_end_paragraph = 0;
        }
        // clear help_buff, but do it only if we will place something inside
        if (help_buff[0] != L'\0') {
            wcscpy(buffer, help_buff);
            // reset our buffer
            memset(help_buff, L'\0', sizeof(help_buff));
        }

        // if everything is clear paragraph recently ended place ident at the beginning of the new buffer
        int placed_indent = 0;
        // IDENT
        if (wcslen(buffer) == 0){
            for (int j = 0; j < ident; ++j) {
                buffer[j] = L' ';
            }
            placed_indent = 1;
        }

        // give more text only if it's possible and we don't risk buffer overflow
        if (i < wcslen(input) && help_buff_len < N*2 && is_end_paragraph == 0){
            // shortened input only if first line
            if (wcslen(buffer) == 0){
                prepare_text(&input[i], N-ident, &finishedPreparing, buffer);
            }else {
                prepare_text(&input[i], N, &finishedPreparing, buffer);
            }

            // we finish at different indexes in every part of input, that's why wy have +=
            i += finishedPreparing;
        }

//        printf("%ls\n", buffer);

        // go from the end of buffer to N and add every char to the buffer_additional
        // take care of extensive string
        // TRIM CERTAINLY TOO LONG
        int ind = wcslen(buffer) - 1;
        // get rid of characters that are after N
        while (ind > N) {
            // shift string by one to the right
            // make place for new char
            for (size_t c = wcslen(help_buff); c > 0; c--) {
                help_buff[c] = help_buff[c - 1];
            }
            // place a new char
            help_buff[0] = buffer[ind];
            // clear buffer after adding element
            buffer[ind] = L'\0';
            ind--;
        }

//        // get rid of chars that come from not-fitting words
//        // TRIM OVERFLOWING STRING
        if (!isspace(buffer[N] && buffer[N] != L'\0')) {
            // check how long is the word that exceeds the length
            ind = N;
            while (!isspace(buffer[ind]) && ind >= 0) {
                // move buff_help to the right
                for (size_t c = wcslen(help_buff); c > 0; c--) {
                    help_buff[c] = help_buff[c - 1];
                }
                help_buff[0] = buffer[ind];
                buffer[ind] = L'\0';
                ind--;
            }
        }
//
//        // WHOLE-LINER CASE
        if (ind + 1 == 0) {
            // bring the word back to the buffer, while adding '-' at the end
            for (int w = 0; w < N - 1; w++) {
                buffer[w] = help_buff[w];
                help_buff[w] = L'\0'; // clear buffer additional
            }
            buffer[N - 1] = L'-';
            // fix help_buff, shift text to the left
            int shift_index = 0;
            while (help_buff[shift_index + (N-1)] != L'\0') {
                help_buff[shift_index] = help_buff[shift_index + (N-1)];
                help_buff[shift_index + (N-1)] = L'\0'; // clear end after shifted
                shift_index++;
            }
        }
        buffer[N+1] = L'\0';
//        printf("%ls \n", buffer);
//
//        // PROPER SPACE DIVISION
        if (placed_indent) {
            givingResult(buffer, N-ident, even, ident, alignment);
        }else {
            givingResult(buffer, N, even, 0, alignment);
        }

        even *= -1;
        memset(buffer, L'\0', 3 * N);
    }
}

void infoWords(wchar_t buffer[], int N, int *words_counter, int *letters_counter){
    int ind = 0;
    while (ind < N){
        // if it's a letter
        if (isspace(buffer[ind]) == 0 && buffer[ind] != L'\0'){
            int w_ind = ind;
            // go to the end of encountered word
            while (isspace(buffer[w_ind]) == 0 &&  w_ind < N){
                w_ind++;
                ++(*letters_counter);
            }
            ++(*words_counter);
            ind = w_ind;
        }else {
            ind++;
        }
    }
}

void givingResult(wchar_t buffer[], int N, int even, int indent, int alignment){
    // count words
    wchar_t *result = calloc(N*3, sizeof(wchar_t ));
    int words_counter = 0;
    int letters_counter = 0 ;
    infoWords(buffer, N+indent, &words_counter, &letters_counter);

    int holes = words_counter - 1;
    // true amount of needed spaces is that without obligatory spaces
    letters_counter += holes;
    int spaces_available = N - letters_counter;
    int spaces_per_hole = 0;
    int additional_spaces = 0;
    // COUNT HOLES
    if (holes > 0){
        spaces_per_hole = spaces_available / holes;
        additional_spaces = spaces_available % holes;
    }else {
        spaces_per_hole = spaces_available / 2;
        additional_spaces = spaces_available % 2;
    }

    // if row is the last in the paragraph
    int is_last_row = 0;
    int len = wcslen(buffer);
    if (buffer[len-1] == L'\n'){
        is_last_row = 1;
    }

    if (is_last_row == 0) {
        // TAKING CARE OF PROPER JUSTIFICATION
        blowSpaces(result, buffer, words_counter, indent, even, alignment, N, holes, spaces_per_hole, additional_spaces, letters_counter);
    }else {
        // LAST ROW ALIGNMENT
        lastRowAlignment(buffer, result, N+indent, spaces_available, spaces_per_hole, additional_spaces, is_last_row, alignment);
    }

    // normal end justification
    if (is_last_row == 1 && alignment == 3){
        wchar_t br[3] = L"\n\n\0";
        wcscat(result, br);
    }
    printf("%ls\n", result);

    free(result);


}
