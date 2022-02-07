

#include "privateJsonHeader.h"


int json_valid( char * ftry ) {
json_parser parser;
int         i;

    json_init(&parser);
    i = json_parse(&parser, ftry, strlen(ftry), NULL, 0);
    if ( ( i == json_ERROR_INVAL ) || ( i == json_ERROR_PART ) ) {
       return FALSE;
    }
    return TRUE;
}
