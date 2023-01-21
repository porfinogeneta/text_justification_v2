#include "headers.h"
#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include "stdlib.h"

void prepare_text(char input[], int N, int *finishedPreparing, char *buffer) {
//    printf("%s\n", input);
//    printf("=================================\n");
    // memory allocation
    char *bufferNewLines = calloc(3 * N, sizeof(char));
    char *bufferNoSpaces = calloc(3 * N, sizeof(char));
    int index = 0;
    int letters = 0;
    // take care of \n
    // N+1 because we want to see what is after the end
    while (letters < N + 1) {
        if (input[index] == '\n') {
            int amount = 0; // amount of spaces
            int ind = index;
            while (input[ind] == '\n') {
                amount++;
                ind++;
            }
            if (amount == 1) {
                char oneSpace = ' ';
                strncat(bufferNewLines, &oneSpace, 1);
//                printf(" ");
            }

            if (amount >= 2 && index != 0) {
                strcat(bufferNewLines, "\n\n");
//                printf("\n\n");
            }
            // side-case spaces
            if (input[ind] == ' ') {
                // before was \n
                // take care of spaces after \n\n
                if (input[ind - 1] == '\n') {
                    ++ind;
                }
            }
            // take care of spaces before \n\n

            index = ind;
        }
        if (!isspace(input[index])) {
            letters++;
        }
        strncat(bufferNewLines, &input[index], 1);

        index++;
    }
    char null = '\0';
    strncat(bufferNewLines, &null, 1);

    // take care of additional spaces
    // place where we finished taking care of our text
    *finishedPreparing = index;
    letters = 0;
    index = 0;
    while (letters < N + 1) {
        int space_detected = 0;
        if (bufferNewLines[index] == ' ') {
            space_detected++;
            int ind = ++index;
            while (bufferNewLines[ind] == ' ') {
                space_detected++;
                ind++;
            }

            char oneSpace = ' ';
            // don't add space if the paragraph ends
            if (bufferNewLines[ind+1] != '\n'){
                strncat(bufferNoSpaces, &oneSpace, 1);
            }

            index = ind;
        }


        if (!isspace(input[index])) {
            letters++;
        }
        strncat(bufferNoSpaces, &bufferNewLines[index], 1);
        index++;
    }
//    printf("%s", bufferNoSpaces);
    strcat(buffer, bufferNoSpaces);
//    printf("%s\n", bufferNewLines);
    memset(bufferNoSpaces, 0, N*3);
    memset(bufferNewLines, 0, N * 3);
}


void justify_center(char buffer[], char input[], int N){
    // pointer to the index where we finished preparing our text
    int finishedPreparing = 0;
    char help_buff[N*3];
    int even = -1; // is even 1, is not -1
    // clear buffer additional before everything
    for (int j = 0; j < (3 * N) - 1; ++j) {
        help_buff[j] = '\0';
    }
    int i = 0;
    while (i < strlen(input) || help_buff[0] != '\0') {

        // prepare our text to justification
        // copy text not justified text into our buffer to handle it
        int help_buff_len = strlen(help_buff);
        if (help_buff[0] != '\0') {
            strcat(buffer, help_buff);
            // reset our buffer
            memset(help_buff, 0, N*3);
        }
        // give more text only if it's possible and we don't risk buffor overflow
        if (i < strlen(input) && help_buff_len < N*2){
            prepare_text(&input[i], N, &finishedPreparing, buffer);
//        printf("%s\n", buffer);
            // we finish at different indexes in every part of input, that's why wy have +=
            i += finishedPreparing;
        }
        // go from the end of buffer to N and add every char to the buffer_additional
        // take care of extensive string
        // TRIM CERTAINLY TOO LONG
        int ind = strlen(buffer) - 1;
        // get rid of characters that are after N
        while (ind > N) {
            // shift string by one to the right
            // make place for new char
            for (size_t c = strlen(help_buff); c > 0; c--) {
                help_buff[c] = help_buff[c - 1];
            }
            // place a new char
            help_buff[0] = buffer[ind];
            // clear buffer after adding element
            buffer[ind] = '\0';
            ind--;
        }
//        printf("%s\n", buffer);
//        printf("help: %s \n", help_buff);
        // get rid of chars that come from not-fitting words
        // TRIM OVERFLOWING STRING
        if (!isspace(buffer[N] && buffer[N] != '\0')) {
            // check how long is that word that exceeds the length
//            printf("yes\n");
            ind = N;
            while (!isspace(buffer[ind]) && ind >= 0) {
                // move buff_help to the right
                for (size_t c = strlen(help_buff); c > 0; c--) {
                    help_buff[c] = help_buff[c - 1];
                }
                help_buff[0] = buffer[ind];
                buffer[ind] = '\0';
                ind--;
            }
        }


        // WHOLE-LINER CASE
        if (ind + 1 == 0) {
            // bring the word back to the buffer, while adding '-' at the end
            for (int w = 0; w < N - 1; w++) {
                buffer[w] = help_buff[w];
                help_buff[w] = '\0'; // clear buffer additional
            }
            buffer[N - 1] = '-';
            // fix help_buff, shift text to the left
            int shift_index = 0;
            while (help_buff[shift_index + (N-1)] != '\0') {
                help_buff[shift_index] = help_buff[shift_index + (N-1)];
                help_buff[shift_index + (N-1)] = '\0'; // clear end after shifted
                shift_index++;
            }
        }
        buffer[N+1] = '\0';
//        printf("%s", buffer);
        // PROPER SPACE DIVISION
        givingResult(buffer, N, even);
        even *= -1;
        memset(buffer, 0, 3 * N);
    }
}

void infoWords(char buffer[], int N, int *words_counter, int *letters_counter){
    int ind = 0;
    while (ind < N){
        // if it's a letter
        if (isspace(buffer[ind]) == 0 && buffer[ind] != '\0'){
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

void givingResult(char buffer[], int N, int even){
    // count words
    char *result = calloc(N*3, sizeof(char ));
    int words_counter = 0;
    int letters_counter = 0 ;
    infoWords(buffer, N, &words_counter, &letters_counter);

    int holes = words_counter - 1;
    // true amount of needed spaces is that without obligatory spaces
    letters_counter += holes;
    int spaces_available = N - letters_counter;
    int spaces_per_hole = 0;
    int additional_spaces = 0;
    if (holes > 0){
        spaces_per_hole = spaces_available / holes;
        additional_spaces = spaces_available % holes;
    }else {
        spaces_per_hole = spaces_available / 2;
        additional_spaces = spaces_available % 2;
    }

    // NORMAL CASE JUSTIFICATION
    if (words_counter > 1){
        int ind = 0;
        while (ind < N){
            // if we encountered word and didn't exceed word length
            while (isspace(buffer[ind]) == 0 && buffer[ind] != '\0'){
                strncat(result, &buffer[ind], 1);
//                printf("%c", buffer[ind]);
                ind++;
            }
            // if we encountered space and there is an obligatory one
            if (isspace(buffer[ind]) && holes > 0 && ind < N){
                char w = ' ';
                for (int i = 0; i < spaces_per_hole; ++i) {
//                    printf(" ");
                    strncat(result, &w, 1);
                }
                holes--;
//                printf(" "); // printing obligatory space
                strncat(result, &w, 1);
            }
            // distributing additional_spaces
            ind++;
        }
    }
    // ONE WORD FITTING CASE
    else {
        // it's a line that can accept some spaces
        if (letters_counter <= N){
            int ind = 0;
            char s = ' ';
            for (int i = 0; i < spaces_per_hole; ++i) {
//                printf(" ");
                strncat(result, &s, 1);
            }
            if (additional_spaces > 0 && even == 1){
                strncat(result, &s, 1);
            }
            while (isspace(buffer[ind]) == 0 && buffer[ind] != '\0'){
//                printf("%c", buffer[ind]);
                strncat(result, &buffer[ind], 1);
                ind++;
            }
            for (int i = 0; i < spaces_per_hole; ++i) {
                strncat(result, &s, 1);
//                printf(" ");
            }
            if (additional_spaces > 0 && even == -1){
//                printf(" ");
                strncat(result, &s, 1);
            }
        }
    }


    // take care of additional spaces
    int additional_len = strlen(result);
    int index = additional_len-1;
    // in even rows
    if (even == 1){
        while (additional_spaces > 0 && words_counter > 1) {
            // space was found
            if (isspace(result[index])) {
                int ind = index;
                while (isspace(result[ind])) {
                    ind--;
                }
                // shift right whole result to make place for additional space
                for (int c = N-1; c > ind; c--) {
                    result[c] = result[c - 1];
                }
                // insert space
                result[ind+1] = ' ';
                additional_spaces--;
            }else {
                index--;
            }
        }
    }else {
        index = 0;
        while (additional_spaces > 0 && words_counter > 1) {
            // space was found
            if (isspace(result[index])) {
                int ind = index;
                while (isspace(result[ind])) {
                    ind++;
                }
                // shift right whole result to make place for additional space
                for (int c = N-1; c >= ind; c--) {
                    result[c] = result[c - 1];
                }
                // insert space
                result[ind] = ' ';
                additional_spaces--;
            }else {
                index++;
            }
        }
    }

    printf("%s\n", result);

    free(result);


}
