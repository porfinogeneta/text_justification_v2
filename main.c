#include "headers.h"
#include "stdio.h"
#include "wchar.h"
#include "locale.h"
#include "stdlib.h"

// troubleshooting
// no spaces validation if index is first

#define N 20
#define IDENT  2
#define LAST 2

int main(){
    // ensure UTF-8 encoding
    setlocale(LC_ALL, "pl_PL.utf8");

    wchar_t input[2000] = L"Dom wariatów – obraz olejny hiszpańskiego malarza Francisca Goi. Dzieło przedstawia scenę rozgry bwającą się w domu dla obłąkanych. Znajdują się w nim nagie i półnagie groteskowe postaci.\n\n\nIch stroje, gesty i działania są dowodem obłąkania, które doprowadziło ich do zamknięci\nw tym miejscu. Obraz jest alegorią powszechnego szaleństwa, która obejmuje także tych, którzy rządzą światem: króla, papieża oraz wojskowych.\n\n Z drugiej strony, to właśnie ci uwięzieni są prawdziwie wolni \n\n\0";
    wchar_t *buffer = calloc(3*N, sizeof(wchar_t ));
//    char *buffer_additional = calloc(3*N, sizeof(char ));

    justify_center(buffer, input, N, LAST, IDENT);


    return 0;
}
