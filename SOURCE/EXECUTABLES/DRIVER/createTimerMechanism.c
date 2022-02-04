/**
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          createTimerMechanism.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021 
 *
 * @brief         This function creates and manages the device sample frequencies  
 *
 * Each device managed by this driver may have a different sampleFrequency and we 
 * need a mechanism to ensure that the devices get serviced at the specified 
 * frequncies.
 * Normally each of the enclosed functions would be documented in a separate file
 * but since they work together and are part of an unusual mechanism, I thought 
 * it best to put them all here in one place. 
 *        
 *  
 * 
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
 
 #include "privateDeviceDriver.h"
 #include <sys/time.h>
 
 typedef struct _timerControl {
                                int      timeRemaining;     /* In microseconds */
                                int      originalFrequency; /* In microseconds */
                                device * owner;             /* Owner Device    */
                              } timerControl;
                              
 timerControl * timerArray = NULL;                      /* Dynamic Array  */
 
 
 void createTimer( device * D ) {
 timerControl tC;
 
    if ( timerArray == NULL ) {
       timerArray = dynarray_create( timerControl );
    }
    tC.timeRemaining     = D->sampleFrequency;
    tC.originalFrequency = D->sampleFrequency;
    tC.owner             = D;
    
    dynarray_push( timerArray, tC );
 }
 
void destroyTimers( ) {
    if ( timerArray != NULL ) {
       dynarray_destroy( timerArray );
       timerArray = NULL;
    }
}

 int timerControlComparator( const void * p1, const void * p2 ) {
 const timerControl *t1,*t2;
   t1 = ( timerControl * ) p1;
   t2 = ( timerControl * ) p2;
   
   return ( t1->timeRemaining > t2->timeRemaining );
 }
 
 void tickTock( int sig ) {
 struct itimerval newInterval;
 device * device;
 int  timeElapsed,i,saveError;
 char notice[20];
 /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * When this signal SIGALRM is received it means that the timeInterval 
  * of the first device on the queue has elapsed. So we take this time
  * away from everyone else on the queue ...
  * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  */
  timeElapsed = timerArray[0].timeRemaining;
  for( i = 0; i < dynarray_length( timerArray ); i++ ) {
     timerArray[i].timeRemaining -= timeElapsed;
  }
  /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   * Anyone on the queue whose timeRemaining is 0 or less needs to be 
   * notified that it is time to service the device and their timerControl
   * needs to be reset to the sampleFrequency...
   * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   */
   for( i= 0; i < dynarray_length( timerArray ); i++ ) {
      if ( timerArray[i].timeRemaining <= 0 ) {
         timerArray[i].timeRemaining = timerArray[i].originalFrequency;
         device = timerArray[i].owner;
         sprintf( notice,"T%03d", device->index );
         write( device->devicePipe[1], notice, strlen( notice ) );
         terminationReason = UNKNOWN_REQUEST;
      }
   }
   /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    * Sort the time queue again and start the smallest timer
    * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    */
   qsort( timerArray, 
          dynarray_length( timerArray ), 
          sizeof( timerControl ), 
          timerControlComparator ); 
   if ( dynarray_length( timerArray ) != 0 ) {
      newInterval.it_value.tv_sec  = timerArray[0].timeRemaining / 1000;
      newInterval.it_value.tv_usec = timerArray[0].timeRemaining % 1000;
      
      newInterval.it_interval.tv_sec     = 0;  /* One shot timer  */
      newInterval.it_interval.tv_usec    = 0;
      
      i = setitimer( ITIMER_REAL, &newInterval, NULL );
      if ( i == -1 ) {
        saveError = errno;
        ARGOS_PRINTF("Unable to create interval timer\n");
        ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
        ARGOS_PRINTF("Unable to continue ...\n");
        exit(4);
      }
   }
 }
  
 void armTimer( void ) {
 struct itimerval newInterval;
 struct sigaction sact;
 int              j,saveError;
 
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Make sure the list is in non-decreasing order ...
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    qsort( timerArray, 
           dynarray_length( timerArray ), 
           sizeof( timerControl ), 
           timerControlComparator );
        
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Set up the handler for SIGALRM signals ...
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   sigemptyset( &sact.sa_mask );
   sact.sa_flags   = 0;
   sact.sa_handler = tickTock;
   sigaction( SIGALRM, &sact, NULL );
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * If we have time intervals on the queue, the one at the top
 * will be used to set the timer ...
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   if ( dynarray_length( timerArray ) != 0 ) {
      newInterval.it_value.tv_sec  = timerArray[0].timeRemaining / 1000;
      newInterval.it_value.tv_usec = timerArray[0].timeRemaining % 1000;
      
      newInterval.it_interval.tv_sec     = 0;  /* One shot timer  */
      newInterval.it_interval.tv_usec    = 0;
      
      j = setitimer( ITIMER_REAL, &newInterval, NULL );
      if ( j == -1 ) {
        saveError = errno;
        ARGOS_PRINTF("Unable to create interval timer\n");
        ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
        ARGOS_PRINTF("Unable to continue ...\n");
        exit(4);
      }
   }    
 }             
 
