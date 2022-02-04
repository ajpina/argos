extern char  argosJunkString[1024];
extern char  argosWorkString[1024];

#include "argos.h"

int ARGOS_FPRINTF( FILE * f, const char * fmt, ... ) {
va_list args;
int     i;

    memset( argosJunkString, '\0', 1024 ); 
    va_start( args, fmt );
    if ( ARGOS_IS_DAEMONIZED ) {
       i = sprintf( argosJunkString, fmt, args );
       if ( strchr( argosJunkString, '\n' ) == NULL ) {
          strcat( argosWorkString, argosJunkString );
          memset( argosJunkString, '\0', 1024 );
       } else {
          strcat( argosWorkString, argosJunkString );
          syslog(ARGOS_LOG, "%s", argosWorkString );
          i = strlen( argosWorkString );
          memset( argosWorkString, '\0', 1024 );
          memset( argosJunkString, '\0', 1024 );
       }
    } else {
       i = vfprintf( f, fmt, args );
    }
    va_end( args );
    return i;
}

