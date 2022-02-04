/**
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          setupDevice.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021 
 *
 * @brief         Prepares the device for operation 
 *
 *
 * @param         pointer to device control information             
 * @return        nothing
 * 
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
 
 #include "privateDeviceDriver.h"
 
 void setupDevice( device * D ) {
 int       j,saveError;
 char    * shmName = NULL;
 char    * mqName  = NULL;

 
 
   if ( D->enabled == 'N' ) return;
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * We start by ensuring that all the options we need are specified
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    if ( D->deviceName == NULL ) {
	ARGOS_PRINTF("Device %d does not have a name!\n",D->index);
	D->enabled = 'N';
	return;
    }
    if ( D->sampleFrequency == 24 * 60 * 60 * 1000 ) {
       ARGOS_PRINTF("Device %d is using the default frequency of once per day!\n",D->index);
    }


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Create the Shared Memory Segment for each device and map it into our
 * address space so we can get at it conveniently ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */ 
   shmName = ( char * ) calloc( 128, 1 );
   memset( shmName, '\0', 128 );
   sprintf( shmName,"/ARGOS_SHARED_DEVICE_%03d",D->index );
   if ( D->enabled == 'Y' ) {
	j = shm_open( shmName, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO );
	if ( j == -1 ) {   /* An error occured ... */
	   saveError = errno;
	   ARGOS_PRINTF("Unable to create shared memory segment %s\n",shmName);
	   ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
	   ARGOS_PRINTF("Disabling device ...\n");
	   D->enabled = 'N';
	} else {
	   ftruncate( j, D->shm_size_required );
	   D->shmBlock = mmap(0, 
	                      D->shm_size_required, 
	                      PROT_READ|PROT_WRITE, 
	                      MAP_SHARED, 
	                      j, 
	                      (long)0
	                     );
	   close( j );
	   memset( D->shmBlock, '\0', D->shm_size_required );
	}             
   } else {
	j = shm_unlink( shmName );
	if ( j == -1 )  {
	   saveError = errno;
	   if ( saveError != ENOENT ) {
	     ARGOS_PRINTF("Unable to unlink shared memory segment %s\n",shmName);
	     ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
	     ARGOS_PRINTF("Continuing ...\n");
	   }
	}
   }
   free( shmName );
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Open a message queue for the WRITER process to communicate with us ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */    
    mqName = ( char * ) calloc( 128, 1 );
    memset( mqName, '\0', 128 );
    sprintf( mqName, "/ARGOS_MSGQUEUE_DEVICE_%03d",D->index );
    if ( D->enabled == 'Y' ) {
         j = ( int ) mq_open( mqName, 
                              O_CREAT | O_RDONLY | O_NONBLOCK , S_IRWXU | S_IRWXG | S_IRWXO,
                              NULL );
         if ( j == -1 ) {   /* An error occured ... */
           saveError = errno;
           ARGOS_PRINTF("Unable to create message queue %s\n",mqName);
           ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
           ARGOS_PRINTF("Unable to continue ...\n");
           exit(4);
         } else {
           D->mQ = ( mqd_t )j;
         }
    } else {
         j = mq_unlink( mqName );
         if ( j == -1 ) {
           saveError = errno;
           if ( saveError != ENOENT ) {
             ARGOS_PRINTF("Unable to delete message queue %s\n",mqName);
             ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
             ARGOS_PRINTF("Continuing ...\n");
           }
         }
    }
    free( mqName );    
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Now is the time to try and connect to the physical devices ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */ 
    if ( D->enabled == 'Y' ) {
       D->openConnection( D );
       if ( D->connected == 'N' ) {
          ARGOS_PRINTF("Disabling Unconnected Device  %d  %s\n",
                 D->index, D->deviceName ); 
          D->enabled = 'N';
       }
    }
    
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Since each device may have a different sampleFrequency we need to
 * implement a timing mechanism to work with ualarm and SIGALRM signals
 * to keep track of the various sampling needs ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    createTimer( D );   
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Create a pipe so that the signal handlers can send us events ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */     
    j = pipe( D->devicePipe );
    if ( j == -1 ) {
       saveError = errno;
       ARGOS_PRINTF("Unable to create pipe for device %d\n", D->index );
       ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
       ARGOS_PRINTF("Unable to continue ...\n");
       exit(4);
    }
 }
 
