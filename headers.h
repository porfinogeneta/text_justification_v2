//
// Created by szymonm on 12/01/23.
//

#ifndef TEXT_JUSTIFICATION_V2_HEADERS_H

void prepare_text(char text[], int N, int *finishedPreparing, char *buffer);
void justify_center(char buffer[], char input[], int N, int alignment);
void givingResult(char buffer[], int N, int even, int paragraph_counter, int alignment);




#define TEXT_JUSTIFICATION_V2_HEADERS_H

#endif //TEXT_JUSTIFICATION_V2_HEADERS_H
