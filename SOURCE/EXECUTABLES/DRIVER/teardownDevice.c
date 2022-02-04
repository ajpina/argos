/**
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          tearDownDevice.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021 
 *
 * @brief         disables the device from further operation 
 *    
 * @param         pointer to device control information             
 * @return        nothing
 * 
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
 
 #include "privateDeviceDriver.h"
 
 void teardownDevice( device * D ) {
 char    * shmName = NULL;
 char    * mqName  = NULL;
 int       j,saveError;
 /* ++++++++++++++++++++++++++++++++++++++++++++++
  * Close the Device connection ...
  * ++++++++++++++++++++++++++++++++++++++++++++++
  */
     D->closeConnection( D );
 /* ++++++++++++++++++++++++++++++++++++++++++++++
  * Unmap the shared memory and unlink it
  * ++++++++++++++++++++++++++++++++++++++++++++++
  */
     j = munmap( D->shmBlock, D->shm_size_required );
     
     shmName = ( char * ) calloc( 128, 1 );
     memset( shmName, '\0', 128 );
     sprintf( shmName,"/ARGOS_SHARED_DEVICE_%03d",D->index );
     j = shm_unlink( shmName );
     if ( j == -1 )  {
	   saveError = errno;
	   if ( saveError != ENOENT ) {
	     ARGOS_PRINTF("Unable to unlink shared memory segment %s\n",shmName);
	     ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
	     ARGOS_PRINTF("Continuing ...\n");
	   }
     }
     free( shmName );
/* +++++++++++++++++++++++++++++++++++++++++++++++++
 * Close and nlink the message queue
 * +++++++++++++++++++++++++++++++++++++++++++++++++
 */
     mq_close( D->mQ );
     mqName = ( char * ) calloc( 128, 1 );
     memset( mqName, '\0', 128 );
     sprintf( mqName, "/ARGOS_MSGQUEUE_DEVICE_%03d",D->index );
     j = mq_unlink( mqName );
     if ( j == -1 ) {
           saveError = errno;
           if ( saveError != ENOENT ) {
             ARGOS_PRINTF("Unable to delete message queue %s\n",mqName);
             ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
             ARGOS_PRINTF("Continuing ...\n");
           }
     }
     free( mqName );  
/* +++++++++++++++++++++++++++++++++++++++++++++
 * CLose both ends of the pipe ...
 * +++++++++++++++++++++++++++++++++++++++++++++
 */
     close( D->devicePipe[0] );
     close( D->devicePipe[1] );
     D->destroyDevice( D );
  
 }
 
