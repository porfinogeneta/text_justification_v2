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

//     clear buffer additional
        for (int j = 0; j < (3 * N) - 1; ++j) {
            help_buff[j] = '\0';
        }
    for (size_t i = 0; i < strlen(input);) {

        // prepare our text to justification
        // copy text not justified text into our buffer to handle it
        if (help_buff[0] != '\0') {
            strcat(buffer, help_buff);
            // reset our buffer
            memset(help_buff, 0, N*3);
        }
        prepare_text(&input[i], N, &finishedPreparing, buffer);
//        printf("%s\n", buffer);

        // we finish at different indexes in every part of input, that's why wy have +=
        i += finishedPreparing;

        // go from the end of buffer to N and add every char to the buffer_additional
        // take care of extensive string
        size_t ind = strlen(buffer) - 1;
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
        printf("%s\n", buffer);
        printf("help: %s \n", help_buff);
        // get rid of chars that come from not-fitting words
        if (!isspace(buffer[N] && buffer[N] != '\0')) {
            // check how long is that word that exceeds the length
//            printf("yes\n");
            ind = N;
            while (!isspace(buffer[ind]) && ind > 0) {
                // move buff_help to the right
                for (size_t c = strlen(help_buff); c > 0; c--) {
                    help_buff[c] = help_buff[c - 1];
                }
                help_buff[0] = buffer[ind];
                buffer[ind] = '\0';
                ind--;
            }
        }

//         case for whole line word
//        if (ind + 1 == 0) {
//            // bring the word back to the buffer, while adding '-' at the end
//            for (int w = 0; w < N - 1; w++) {
//                buffer[w] = buffer_additional[w];
//                buffer_additional[w] = '\0'; // clear buffer additional
//            }
//            buffer[N - 1] = '-';
//            // fix buffer additional, shift text to the left
//            int shift_index = 0;
//            while (shift_index < strlen(buffer)) {
//                buffer_additional[shift_index] = buffer_additional[shift_index + N];
//                buffer_additional[shift_index + N] = '\0'; // clear end after shifted
//                shift_index++;
//            }
//        }
//        printf("\n%s ", buffer_additional);
//        // get to know what words should be in the buffer
//        int buff_addi_len = strlen(buffer_additional);
//        // clear everything after counted index, including index
//        int dif = strlen(buffer) - buff_addi_len;
//        while (buffer[dif] != '\0') {
//            buffer[dif] = '\0';
//            dif++;
//        }
//        // get rid of possible ending space
//        if (isspace(buffer[strlen(buffer) - 1])) {
//            buffer[strlen(buffer) - 1] = '\0';
//        }
//        // now let's count how many spaces do we have
//        ind = 0;
//        int holes = 0;
//        while (ind < strlen(buffer) - 1) {
//            if (isspace(buffer[ind])) {
//                holes++;
//            }
//            ind++;
//        }

//        // spaces in each hole
//        int spaces_in_each = N - strlen(buffer);
//        // extra spaces
//        int extra_spaces = spaces_in_each;
//        if (holes != 0) {
//            spaces_in_each = spaces_in_each / holes;
//            extra_spaces = spaces_in_each % holes;
//        }
//        // PRINTING
////        // case for one-word fitting
//        if (holes == 0) {
//            // equally distribute spaces
//            int amount = spaces_in_each / 2;
//            int additional = spaces_in_each % 2;
//            for (int j = 0; j < amount; ++j) {
//                printf(" ");
//            }
//            if (additional) printf(" ");
//            printf("%s", buffer);
//            for (int j = 0; j < amount; ++j) {
//                printf(" ");
//            }
//        }
//        printf("\n");

//        // if only one word fits
//        for (int j = 0; j < strlen(buffer); ++j) {
//            if (isspace(buffer[j])) {
//                int to_give = spaces_in_each;
//                printf(" ");
//                while (to_give > 0) {
//                    printf(" ");
//                    to_give--;
//                }
//                if (extra_spaces > 0) {
//                    printf(" ");
//                    extra_spaces--;
//                }
//            } else {
//                // if it's not a one word fitting
//                if (holes != 0) {
//                    printf("%c", buffer[j]);
//                }
//            }
//        }
//        printf("\n");
        memset(buffer, 0, 3 * N);
    }
}
