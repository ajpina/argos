/**
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 *                        >>> ARGOS-SCADA <<<
 *                        Copyright (c) 2021
 *
 * @file    main.c  is a part of the ARGOS_SCADA system
 * @author  Wayne Collins  wcol0952A@gmail.com
 *
 * @brief   This is a driver for all the system's devices
 *
 * This program will support and collect data from many devices
 * which use any of the supported protocols.
 *
 * This program periodically gets data from a device with a
 * controllable frequency. The collected data is placed into a
 * shared memory segment where it will be processed by another
 * ARGOS program. In addition, the program periodically gets
 * data instructions via a message queue which it must relay to
 * the device. Each device has a unique shared memory segment
 * but there is only 1 message queue for the controller.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

#include "privateDeviceDriver.h"

/* ++++++++++++++++++++++++++++++++++++++++++++++
 * Switch to control whether or not we will
 * daemonize if all plugins are loaded ...
 * ++++++++++++++++++++++++++++++++++++++++++++++
 */
char daemonizeControl   = 'N';
int  terminationReason;
int  enabledDeviceCount = 0;

/* ++++++++++++++++++++++++++++++++++++++++++++++
 * Every program needs one of these ...
 * ++++++++++++++++++++++++++++++++++++++++++++++
 */
struct rlimit    rlimits;

int main( int argc, char * argv[ ] ) {
sigset_t        ourSignals;



/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Make sure we get a valid core file if this program has problems
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   rlimits.rlim_cur = RLIM_INFINITY;
   rlimits.rlim_max = RLIM_INFINITY;
   setrlimit( RLIMIT_CORE, &rlimits );
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * This is NOT enough on Linux. You also need to use the command
 * "ulimit -c unlimited". There does not seem to be a way to do this
 * programatically!!
 *
 * Also you need to tell the Linux kernel that you want core file
 * generated ... by using this command
 * sudo sysctl -w kernel.core_pattern="core"
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Process the command line arguments ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    processCommandLine( argc, argv );

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * (Re-)Process the configuration file
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

reconfig :
    processConfiguration();
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Get all the devices ready ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

   for( int i = 0; i < dynarray_length( configuredDevices ); i++ ) {
      configuredDevices[i].index = i;
      setupDevice( &configuredDevices[i] );
   }
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Check that all the devices configured properly. If not ask the opeator
 * if we should proceed ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    enabledDeviceCount = 0;
    for (int i = 0; i < dynarray_length( configuredDevices ) ; i++ ) {
       if ( configuredDevices[i].enabled == 'N' ) enabledDeviceCount++;
    }
    if ( enabledDeviceCount != dynarray_length( configuredDevices ) ) {
       ARGOS_PRINTF("%d devices did not configure properly!\n",
                    dynarray_length( configuredDevices ) - enabledDeviceCount );
       ARGOS_PRINTF("Should we continue? y|N ...");
       char response = ( char ) toupper( getchar( ) );
       if ( response != 'Y' ) exit(0);
    }
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * If we are going to become a daemon, now is the time to do so ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    if ( daemonizeControl == 'Y' ) daemonize();

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * We are about to create a number of threads and we do NOT want the
 * threads to be handling signals which will be handled by the main
 * thread. So we block them, create the threads and then unblock them
 * again so the main thread can handle them ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    sigemptyset( &ourSignals );
    sigaddset( &ourSignals, SIGUSR1 );
    sigaddset( &ourSignals, SIGHUP  );
    sigaddset( &ourSignals, SIGALRM );

    sigprocmask( SIG_BLOCK, &ourSignals, NULL );

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Everything is set up so let's start all the deviceManagers
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

    terminationReason = UNKNOWN_REQUEST;

    for( int j,i = 0; i < dynarray_length( configuredDevices ); i++ ) {
       if ( ( configuredDevices[i].enabled   == 'Y' )   &&
            ( configuredDevices[i].connected == 'Y' )       ) {
            j = pthread_create( &configuredDevices[i].thread,
                                NULL,
                                configuredDevices[i].manageDevice,
                                (  void * ) &configuredDevices[i] );
            if( j !=  0 ) {  // Not succesful
               ARGOS_PRINTF("Can not start deviceManager thread for device %d\n",i);
               ARGOS_PRINTF("Error  %d  %s\n",j,strerror( j ) );
               exit( 4 );
            }
       }
    }

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * UN_BLOCK the signals that we want the main thread to handle ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

    sigprocmask( SIG_UNBLOCK, &ourSignals, NULL );

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * set up the signal handlers ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    setupSignalHandlers();

    armTimer();

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * We wait patiently ( possibly forever ) for all the threads to end
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    for( int j,i = 0; i < dynarray_length( configuredDevices ); i++ ) {
       if ( ( configuredDevices[i].enabled   == 'Y' )   &&
            ( configuredDevices[i].connected == 'Y' )       ) {
            j = pthread_join( configuredDevices[i].thread, NULL );
            if( j !=  0 ) {  // Not succesful
               ARGOS_PRINTF("Can not join deviceManager thread for device %d\n",i);
               ARGOS_PRINTF("Error  %d  %s\n",j,strerror( j ) );
               ARGOS_PRINTF("Continuing ...\n");
               pthread_cancel( configuredDevices[i].thread );
            }
       }
    }

    disarmSignals();
    destroyTimers();

    for( int i = 0; i < dynarray_length( configuredDevices ); i++ ) {
       teardownDevice( &configuredDevices[i] );
    }

    dynarray_destroy( configuredDevices );

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Why did the threads terminate?
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
    if ( terminationReason == RECONFIGURE_REQUEST ) {
        configuredDevices = NULL;
        goto reconfig;
    }



    return 0;
}


