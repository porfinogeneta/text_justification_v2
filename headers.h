#include "wchar.h"

#ifndef TEXT_JUSTIFICATION_V2_HEADERS_H

void prepare_text(wchar_t text[], int N, int *finishedPreparing, wchar_t *buffer, int ident);
void justify_center(wchar_t buffer[], wchar_t input[], int N, int alignment, int ident);
void givingResult(wchar_t buffer[], int N, int even, int ident, int alignment);
void blowSpaces(wchar_t result[], wchar_t buffer[], int words_counter, int ident, int even, int is_last_row, int alignment, int N, int holes, int spaces_per_hole, int additional_spaces, int letters_counter);
void lastRowAlignment(wchar_t buffer[], wchar_t result[], int spaces_available, int spaces_per_hole, int additional_spaces, int is_last_row, int alignment);
void additionalSpacesDistribution(wchar_t result[], int even, int additional_spaces, int words_counter, int N);




#define TEXT_JUSTIFICATION_V2_HEADERS_H

#endif //TEXT_JUSTIFICATION_V2_HEADERS_H
