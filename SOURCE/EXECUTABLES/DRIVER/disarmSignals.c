 #include "privateDeviceDriver.h"

void disarmSignals( void ) {
 struct sigaction sact;
 int              i, j, saveError;
 
   sigemptyset( &sact.sa_mask );
   sact.sa_flags   = 0;
   sact.sa_handler = SIG_DFL;
   sigaction( SIGHUP, &sact, NULL );
   
   sigemptyset( &sact.sa_mask );
   sact.sa_flags   = 0;
   sact.sa_handler = SIG_DFL;
   sigaction( SIGUSR1, &sact, NULL );
   
   for( i = 0; i < dynarray_length( configuredDevices ); i++ ) {
     if ( configuredDevices[i].enabled   == 'Y' ) {  
        j = mq_notify( configuredDevices[i].mQ, NULL );
        if ( j == -1 ) {
           saveError = errno;
           ARGOS_PRINTF("Unable to setup message queue notification for device %d\n",i);
           ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
           ARGOS_PRINTF("Continuing ...\n");
        }
     }
   }
   
   sigemptyset( &sact.sa_mask );
   sact.sa_flags     = 0;
   sact.sa_handler   = SIG_DFL;
   sact.sa_sigaction = NULL;
   sigaction( SIGUSR2, &sact, NULL );
 }
 
