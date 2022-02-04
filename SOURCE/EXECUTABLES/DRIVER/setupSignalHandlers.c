/**
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          setupSignalHandlers.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021 
 *
 * @brief         Turn On the routines that will handle certain signals 
 *
 * The main program is expecting "events" to arrive via the systemPipe. The 
 * events we will handle are :
 *     1.   SIGLARM generating "T" events (These are handled in the 
 *          tickTock function) see the createTimerMechanism file. 
 *     2.   SIGHUP  generating "Q" events to shut the program down
 *     3.   SIGUSR1 generating "U" events to re-configure the program.
 *     4.   SIGUSR2 generating "M" events showing the arrival of messages
 *          on the WRITER message queue. 
 *        
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
 
 #include "privateDeviceDriver.h"
 
 void shutItDown( int sig ) {
 char * quitEventMessage = "Q   ";
    for( int i = 0; i < dynarray_length( configuredDevices ); i++ ) {
      write( configuredDevices[i].devicePipe[1], 
             quitEventMessage, 
             strlen( quitEventMessage ) );
    }
    terminationReason = TERMINATE_REQUEST;
 }

 void reConfigure( int sig ) {
 char * reconfigureEventMessage = "R   ";
     for( int i = 0; i < dynarray_length( configuredDevices ); i++ ) {
       write( configuredDevices[i].devicePipe[1], 
              reconfigureEventMessage, 
              strlen( reconfigureEventMessage ) );
     }
     terminationReason = RECONFIGURE_REQUEST;
 }
 
 void mqMessageReadyEvent( int sig, siginfo_t * info, void * ucontext ) {
 char             messageArrivalEvent[20];
 int              device;
 struct sigevent  sevt;
 
     device = info->si_value.sival_int;
     memset( messageArrivalEvent, '\0', 20  );
     sprintf( messageArrivalEvent, "M%03d", device );
     write ( configuredDevices[device].devicePipe[1], 
             messageArrivalEvent, 
             strlen( messageArrivalEvent ) );
 
 /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * Re-arm the mq_notify ... 
  * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  */    
     sevt.sigev_notify          = SIGEV_SIGNAL;
     sevt.sigev_signo           = SIGUSR2;
     sevt.sigev_value.sival_int = device;
     mq_notify( configuredDevices[device].mQ, &sevt );
     
     terminationReason = UNKNOWN_REQUEST;
 }
 
  
 void setupSignalHandlers( void ) {
 struct sigaction sact;
 struct sigevent  sevt;
 int              i, j, saveError;
 
   sigemptyset( &sact.sa_mask );
   sact.sa_flags   = 0;
   sact.sa_handler = shutItDown;
   sigaction( SIGHUP, &sact, NULL );
   
   sigemptyset( &sact.sa_mask );
   sact.sa_flags   = 0;
   sact.sa_handler = reConfigure;
   sigaction( SIGUSR1, &sact, NULL );
   
   for( i = 0; i < dynarray_length( configuredDevices ); i++ ) {
     if ( configuredDevices[i].enabled   == 'Y' ) {  
        sevt.sigev_notify          = SIGEV_SIGNAL;
        sevt.sigev_signo           = SIGUSR2;
        sevt.sigev_value.sival_int = i;
        j = mq_notify( configuredDevices[i].mQ, &sevt );
        if ( j == -1 ) {
           saveError = errno;
           ARGOS_PRINTF("Unable to setup message queue notification for device %d\n",i);
           ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
           ARGOS_PRINTF("Continuing ...\n");
        }
     }
   }
   
   sigemptyset( &sact.sa_mask );
   sact.sa_flags     = SA_SIGINFO;
   sact.sa_sigaction = mqMessageReadyEvent;
   sigaction( SIGUSR2, &sact, NULL );
 }
 
