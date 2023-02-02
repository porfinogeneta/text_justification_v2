#include "headers.h"
#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include "stdlib.h"


wchar_t *justify_center(wchar_t buffer[], wchar_t help_buff[], int N, int alignment, int INDENT){
    // pointer to the index where we finished preparing our text
//    int paragraph_counter = 0;
    int even = -1; // is even 1, is not -1
//    printf("%ls\n", buffer);
    while (buffer[0] != '\0') {

        // prepare our text to justification
        // copy text not justified text into our buffer to handle it

        // INDENT
        int placed_indent = 0;
        int INDENT_ind = 0;
        while (buffer[INDENT_ind] == L' '){
            INDENT_ind++;
        }
        if (INDENT_ind >= 1){
            placed_indent = 1;
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

        even *= -1;

        // PROPER SPACE DIVISION
        if (placed_indent) {
            givingResult(buffer, N-INDENT, even, INDENT, alignment);
        }else {
            givingResult(buffer, N, even, 0, alignment);
        }
        // reset buffer
        for (int i = 0; i < N*3; ++i) {
            buffer[i] = L'\0';
        }
    }

    int ind = 0;
    // copy help_buffer to the buffer
//    wmemset(new_buffer, L'\0', N*3);
    while (help_buff[ind] != '\0'){
        buffer[ind] = help_buff[ind];
        ind++;
    }
//    printf("%ls\n", buffer);
    // reset our help_buffer
    for (int i = 0; i < N*3; ++i) {
        help_buff[i] = L'\0';
    }
    return buffer;

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
        blowSpaces(result, buffer, words_counter, indent, even, N, holes, spaces_per_hole, additional_spaces, letters_counter);
    }else {
        // LAST ROW ALIGNMENT
        lastRowAlignment(buffer, result, N+indent, spaces_available, is_last_row, alignment);
    }

    // normal end justification
    if (is_last_row == 1 && alignment == 3){
        wchar_t br[3] = L"\n\n\0";
        wcscat(result, br);
    }
    if (input_place == 0){
        printf("\n\n");
    }
    printf("%ls\n", result);

    free(result);


}
