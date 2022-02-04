#ifndef PRIVATE_DRIVER_HEADER
#define PRIVATE_DRIVER_HEADER

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * 
 *    This is the private header file for the ARGOS device driver
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#include "argos.h"
#include "argosPlugin.h"

#include "json.h"
#include "dynarray.h"

#include <pthread.h>
#include <ctype.h>
                            
              
                 
enum reasonToTerminate {
                            UNKNOWN_REQUEST = 0,
                            TERMINATE_REQUEST,
                            RECONFIGURE_REQUEST
                       };
                                          

                       
               
extern char        * configFilename;
extern char        * pluginDirectory;


extern char          daemonizeControl;       /* Daemon control           */
extern device      * configuredDevices;      /* Dynamic Array of Devices */

extern int           terminationReason;


extern void   processCommandLine(int argc,char * argv[] );
extern void   processConfiguration();
extern void   createTimer( device * D );
extern void   setupSignalHandlers(); 
extern void   armQueueNotifier();
extern void   armTimer();

extern void   disarmSignals();
extern void   closePipe();
extern void   closeMessageQueue(); /* and delete ? */
extern void   destroyTimers();

extern void   setupDevice( device * D );
extern void   teardownDevice( device * D );




#endif
