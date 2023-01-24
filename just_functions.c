#include "headers.h"
#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include "stdlib.h"

void prepare_text(wchar_t input[], int N, int *finishedPreparing, wchar_t *buffer, int ident) {
//    printf("%s\n", input);
//    printf("=================================\n");
    // memory allocation
    wchar_t *bufferNewLines = calloc(3 * N, sizeof(wchar_t));
    wchar_t *bufferNoSpaces = calloc(3 * N, sizeof(wchar_t));
    int index = 0;
    int letters = 0;
    // take care of \n
    // N+1 because we want to see what is after the end
    while (letters < N + 1) {
        if (input[index] == L'\n') {
            int amount = 0; // amount of spaces
            int ind = index;
            while (input[ind] == L'\n') {
                amount++;
                ind++;
            }
            if (amount == 1) {
                wchar_t oneSpace = L' ';
                wcsncat(bufferNewLines, &oneSpace, 1);
//                printf(" ");
            }

            if (amount >= 2 && index != 0) {
                wcscat(bufferNewLines, L"\n\n");
                // finish acquiring text
                index = ind;
                break;
            }
            // side-case spaces
            if (input[ind] == L' ') {
                // before was \n
                // take care of spaces after \n\n
                if (input[ind - 1] == L'\n') {
                    ++ind;
                }
            }
            index = ind;
        }
        if (!isspace(input[index])) {
            letters++;
        }
        wcsncat(bufferNewLines, &input[index], 1);

        index++;
    }
    wchar_t null = L'\0';
    wcsncat(bufferNewLines, &null, 1);

    // take care of additional spaces
    // place where we finished taking care of our text
    *finishedPreparing = index;
    letters = 0;
    index = 0;
    while (letters < N + 1) {
        int space_detected = 0;
        if (bufferNewLines[index] == L' ') {
            space_detected++;
            int ind = ++index;
            while (bufferNewLines[ind] == L' ') {
                space_detected++;
                ind++;
            }

            wchar_t oneSpace = L' ';

            // don't add space if the paragraph ends
            // don't at space if the paragraph just started
            if (bufferNewLines[ind+1] != L'\n' && ( ident != 1 || index != 0 )){
                wcsncat(bufferNoSpaces, &oneSpace, 1);
            }
            // we placed ident, so we don't need one
            if (ident == 1){
                ident = 0;
            }

            index = ind;
        }

        if (!isspace(input[index])) {
            letters++;
        }
        wcsncat(bufferNoSpaces, &bufferNewLines[index], 1);

        if (bufferNewLines[index] == L'\n' && bufferNewLines[index - 1] == L'\n'){
            break;
        }
        index++;
    }
//    printf("%s", bufferNoSpaces);
    wcscat(buffer, bufferNoSpaces);
//    printf("%s\n", bufferNewLines);
    memset(bufferNoSpaces, L'\0', N*3);
    memset(bufferNewLines, L'\0', N * 3);
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
        int placed_ident = 0;
//        // IDENT
//        if (wcslen(buffer) == 0){
//            for (int j = 0; j < ident; ++j) {
//                buffer[j] = L' ';
//            }
//            placed_ident = 1;
//        }

        // give more text only if it's possible and we don't risk buffer overflow
        if (i < wcslen(input) && help_buff_len < N*2 && is_end_paragraph == 0){
            prepare_text(&input[i], N, &finishedPreparing, buffer, placed_ident);
//        printf("%s\n", buffer);
            // we finish at different indexes in every part of input, that's why wy have +=
            i += finishedPreparing;
        }

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
//        printf("%s\n", buffer);
//        printf("help: %s \n", help_buff);
        // get rid of chars that come from not-fitting words
        // TRIM OVERFLOWING STRING
        if (!isspace(buffer[N] && buffer[N] != L'\0')) {
            // check how long is the word that exceeds the length
//            printf("yes\n");
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

        // WHOLE-LINER CASE
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

        // PROPER SPACE DIVISION
        givingResult(buffer, N, even, placed_ident, alignment);
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

void givingResult(wchar_t buffer[], int N, int even, int ident, int alignment){
    // count words
    wchar_t *result = calloc(N*3, sizeof(wchar_t ));
    int words_counter = 0;
    int letters_counter = 0 ;
    infoWords(buffer, N, &words_counter, &letters_counter);

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

    // ADDING MORE SPACES
    blowSpaces(result, buffer, words_counter, ident, even, is_last_row, alignment, N, holes, spaces_per_hole, additional_spaces, letters_counter);


    // LAST ROW ALIGNMENT
    lastRowAlignment(buffer, result, spaces_available, spaces_per_hole, additional_spaces, is_last_row, alignment);


    // ADDITIONAL SPACES
    additionalSpacesDistribution(result, even, additional_spaces, words_counter, N);

    // normal end justification
    if (is_last_row == 1 && alignment == 3){
        wchar_t br[3] = L"\n\n\0";
        wcscat(result, br);
    }
    printf("%ls\n", result);

    free(result);


}
