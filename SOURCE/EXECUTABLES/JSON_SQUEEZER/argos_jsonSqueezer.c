/**
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * @file argos_jsonSqueezer.c
 * @brief This file is part of ARGOS-SCADA
 *
 * Copyright (C) Thursday December 23 2021 
 * @author Wayne Collins
 *
 * ARGOS-SCADA is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ARGOS-SCADA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ARGOS-SCADA. If not, see <http://www.gnu.org/licenses/>.
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */



/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * The ARGOS system program are configured from JSON formatted
 * configuration files. Many of these files in their human-readable
 * form will be VERY large. In order to speed up the processing of
 * these files and to reduce the memory requirements for processing
 * them, we will remove all un-necessary spaces, tabs, returns and
 * LF characters resulting in a JSON formatted file which is still
 * valid JSON syntax, is probably not very human-readable but is
 * much smaller and faster to process.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


#include "argos.h"
#include "json.h"

char * processFilename = NULL;

struct rlimit rlimits;


/*
 *
 * name:   processCommandLine
 * @param  argc and argv as given to main
 * @return void
 *
 */
void processCommandLine( int argc, char * * argv ) {
int moreOptions = 1;
int i;

  while ( moreOptions ) {
    i = getopt( argc, argv, "j:" );
    switch ( i ) {
      case -1  :   /* No more valid options */
                   moreOptions = 0;
                   break;
      case 'j' :   /* File name to process */
                   processFilename = strdup( optarg );
                   break;
    }
  }
  if ( processFilename == NULL ) {
     ARGOS_PRINTF("You did not specify a file to process!\n");
     exit(0);
  }
  if ( optind != argc ) {
     ARGOS_PRINTF("Unrecognized command line options were ignored!\n");
  }
}

 int main( int argc, char * * argv ) {
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Make sure we get a valid core file if this program has problems
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   rlimits.rlim_cur = RLIM_INFINITY;
   rlimits.rlim_max = RLIM_INFINITY;
   setrlimit( RLIMIT_CORE, &rlimits );

   processCommandLine( argc, argv );

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Make sure the file to process exists and is valid JSON
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   if ( access( processFilename, F_OK ) == -1 ) {  /* File does not exist  */
      ARGOS_PRINTF("The configuration file does not exist!\nTerminating ...\n");
      exit(4);
   }
   if ( ! json_validator( processFilename ) ) {
      ARGOS_PRINTF("The file you have supplied for processing is NOT valid json syntax!\n");
      ARGOS_PRINTF("Correct the errors before re-trying ...\n");
      exit( 4 );
   }
   return json_squeezer( "", processFilename );
 };
