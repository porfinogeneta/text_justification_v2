#include "headers.h"
#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include "stdlib.h"

// troubleshooting
// no spaces validation if index is first

#define N 20


int main(){

    char input[2000] = "Sed\n ut persp        iciatisundeom niist esss                  natus error sit voluptatem \n\n\naccusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab\n\n\n illo inventore veritatis\n\0";
    char *buffer = calloc(3*N, sizeof(char ));
//    char *buffer_additional = calloc(3*N, sizeof(char ));

    justify_center(buffer, input, N);


    return 0;
}
