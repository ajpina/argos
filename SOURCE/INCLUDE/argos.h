#ifndef ARGOS_HEADER
#define ARGOS_HEADER
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 *                                      ARGOS Scada System
 *
 * This file contains the manifest constants used globally within the ARGOS project. 
 * This file needs to tailored to a particular installation and then the system can
 * be re-made for that particular configuration.
 * 
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
/* ++++++++++++++++++++++++++++++++++++++++++++++
 * Standard UNIX header files needed by almost
 * every ARGOS program ...
 * ++++++++++++++++++++++++++++++++++++++++++++++
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <mqueue.h>

#include <sys/time.h>
#include <sys/resource.h>


/* ++++++++++++++++++++++++++++++++++++++++++++++
 * Which system log file to use ...
 * This needs to be setup in the OS ...
 * ++++++++++++++++++++++++++++++++++++++++++++++
 */

#define ARGOS_LOG  LOG_LOCAL3     

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * This typedef is needed by most programs in the ARGOS system
 * so we include it here so everyone gets it more or less
 * automatically ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
 
 enum dataTypes {
                  UNKNOWN_TYPE = 0,
                  BIT_TYPE,
                  CHAR_TYPE,
                  SHORT_TYPE,
                  INT_TYPE,
                  LONG_TYPE,
                  FLOAT_TYPE,
                  DOUBLE_TYPE
               };

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * C has no boolean data type so typically we handle bits as char
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
 
typedef union  dataVal {
                          char    C;   /*  1 byte  */
                          short   S;   /*  2 byte  */
                          int     I;   /*  4 byte  */
                          long    L;   /*  8 byte  */
                          float   F;   /*  4 byte  */
                          double  D;   /*  8 byte  */
                       } dataVal;
                          
typedef struct dataValue  {
                               int        dataType;  // From the enum
                               dataVal    value;
                          } dataValue;

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Utility routines useful t many ARGOS programs 
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */ 
 extern int ARGOS_IS_DAEMONIZED;
  
 extern void daemonize( void );
 extern int  ARGOS_PRINTF( const char * fmt, ... ); 
 extern void showMemSizes( void );

#endif
