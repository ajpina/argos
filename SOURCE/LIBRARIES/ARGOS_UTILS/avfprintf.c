#include "argos.h"

extern char  argosJunkString[1024];
extern char  argosWorkString[1024];

int ARGOS_VFPRINTF( FILE * f, const char * fmt, va_list args ) {
int     i;

    memset( argosJunkString, '\0', 1024 ); 

    if ( ARGOS_IS_DAEMONIZED ) {
       i = vsprintf( argosJunkString, fmt, args );
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

    return i;
}
