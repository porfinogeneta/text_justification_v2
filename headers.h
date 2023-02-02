#include "wchar.h"

#ifndef TEXT_JUSTIFICATION_V2_HEADERS_H

// FLAGS
int input_place;


wchar_t *justify_center(wchar_t buffer[], wchar_t help_buff[], int N, int alignment, int indent);
void givingResult(wchar_t buffer[], int N, int even, int indent, int alignment);
void blowSpaces(wchar_t result[], wchar_t buffer[], int words_counter, int indent, int even, int N, int holes, int spaces_per_hole, int additional_spaces, int letters_counter);
void lastRowAlignment(wchar_t buffer[], wchar_t result[], int N, int spaces_available, int is_last_row, int alignment);
void additionalSpacesDistribution(wchar_t result[], int even, int additional_spaces, int words_counter, int N, int indent);




#define TEXT_JUSTIFICATION_V2_HEADERS_H

#endif //TEXT_JUSTIFICATION_V2_HEADERS_H
