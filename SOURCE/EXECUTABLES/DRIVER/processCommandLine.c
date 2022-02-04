 /**
  * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  *
  * @file          processCommandLine.c
  * @author        Wayne Collins
  *          ---   This file is part of the ARGOS_SCADA system   ---
  *                        Copyright (C) 2021 
  *
  * @brief         Process and validate the command line options 
  *
  * This function processes the command line options supplied by the invoker
  * and ensures that a configuration file has been supplied. It also warns of
  * extra optins which are ignored.
  *        
  *
  * @param      argc  count of the number of elements in argv array
  * @param      argv  character strings of the supplied options 
  * @return     nothing     
  * 
  * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  */
  
#include "privateDeviceDriver.h"
  
 
char              * configFilename = NULL;
 
void processCommandLine( int argc, char * * argv ) {
int moreOptions = 1;
int i;

  opterr = 0;        /* We will handle any errors */
  
  while ( moreOptions ) {
    i = getopt( argc, argv, "j:" );
    switch ( i ) {
      case -1  :   /* No more valid options */
                   moreOptions = 0;
                   break; 
      case '?' :   ARGOS_PRINTF("Invalid option %c has been ignored!\n", ( char )optopt );
                   break;
      case 'j' :   /* Configuration file name */
                   configFilename = strdup( optarg );
                   break;
    }
  }
  
#ifdef DEBUG
  configFilename = "/home/wayne/Desktop/ARGOS/DAT/driver.json";
#endif
 
  if ( configFilename == NULL ) {
     ARGOS_PRINTF("You did not specify a configuration file!\n");
     exit(0);
  }

}
