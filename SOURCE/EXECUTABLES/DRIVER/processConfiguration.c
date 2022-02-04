/**
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          processConfiguration.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021 
 *
 * @brief         This funtion extracts configuration data from JSON files 
 *
 * This function reads and parses the JSON configuration specified on the 
 * command line. The information is stored in a globally accesible array
 * called configuredDevices. Data types and Target tpes are verified but all
 * other data is assumed to be of the right type for internal cnversions.
 * Errors in this data will typically result in the program abending.
 *        
 *
 * @param       none
 * @param       none
 * @return      none
 * 
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
#include "privateDeviceDriver.h"
#include <dlfcn.h>

device  *  configuredDevices;
device     workDevice;

char    *  pluginDirectory = NULL;


typedef struct piCB {
                        char   *  devicesName;
                        INIT_RTN  initRoutine;
                    } piCB;   // Plugin COntrol Block

                                  
void processConfiguration( void ) {
char           wkString[512];
char         * wkJunk;

char         * protoName;
char         * carrierName;


int            numberProtocols = 0;

void         * V;
char         * E;

piCB           workPlugin;

piCB         * piCBA;        /* Dynamic Array of Plugin COntrols */

   piCBA = dynarray_create( piCB );
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Make sure the configuration file is valid JSON
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   if ( access( configFilename, F_OK ) == -1 ) {  /* File does not exist  */
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Look for a compressed version of the file
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
      wkJunk = ( char * ) calloc( strlen( configFilename ) + 10, 1 );
      strcpy( wkJunk, configFilename );
      free( configFilename );
      strcat( wkJunk, ".sqzd" );
      configFilename = strdup( wkJunk );
      free( wkJunk );
      if ( access( configFilename, F_OK ) == -1 ) {  /* File does not exist  */
        ARGOS_PRINTF("The configuration file does not exist!\nTerminating ...\n");
        exit(4);
      }
   }

   if ( ! json_validator( configFilename ) ) {
      ARGOS_PRINTF("The configuration file you have supplied is NOT valid json syntax!\n");
      ARGOS_PRINTF("Correct the errors before re-trying ...\n");
      exit( 4 ); 
   }
  
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Process the json configuration file
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */ 
   json_tokenizer( configFilename );
   json_buildDom( );
   
   if ( json_tokenPresent( "daemonize" ) ) {
      daemonizeControl   = json_extract( "$.daemonize" )[0];
   } 
   if ( json_tokenPresent( "plugindirectory" ) ) {
      pluginDirectory    = json_extract( "$.plugindirectory" );
   } 
   if ( pluginDirectory == NULL ) {
     ARGOS_PRINTF("You must supply a PLUGIN directory!\nTerminating\n");
     exit(4);
   }
   
   numberProtocols      = json_array_length("$.protocols");
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Allocate the dynamic array to hold the device descriptions   
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   configuredDevices = ( device * ) dynarray_create( device );
   for( int k = 0; k < numberProtocols; k++ ) {
       sprintf( wkString,"$.protocols[%d].protocol",k );
       protoName = json_extract( wkString );
       sprintf( wkString,"$.protocols[%d].carrier",k );
       carrierName = json_extract( wkString );
       
       sprintf( wkString,"%s/%s_%s.so", pluginDirectory,protoName,carrierName );
/* ++++++++++++++++++++++++++++++++++++++
 * Find and load the plugin
 * ++++++++++++++++++++++++++++++++++++++
 */
       V = dlopen( wkString, RTLD_NOW | RTLD_GLOBAL );
       if ( V == NULL ) {  // Unable to load the shared library
          ARGOS_PRINTF("ERROR in dlopen : %s\n",dlerror() );
          continue;
       }
       
       dlerror();  
       sprintf( wkString,"ARGOS_%s_%s_init",protoName,carrierName );
       workPlugin.initRoutine = dlsym( V, wkString );
       E = dlerror();
       if ( E != NULL ) { 
          ARGOS_PRINTF("Error in dlsym <%s> : %s\n", wkString, E );
          ARGOS_PRINTF("Ignoring this plugin ...\n");  
          continue;
       } 
       
       sprintf( wkString,"$.protocols[%d].devices", k );
       workPlugin.devicesName = json_extract( wkString );

       dynarray_push( piCBA, workPlugin );

   }  /* End For  */
   
   json_finalize();

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Now initialize all the plugins that loaded Ok ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   for( int z = 0; z < dynarray_length( piCBA ); z++ ) {
       piCBA[z].initRoutine( &configuredDevices, piCBA[z].devicesName );
       free( piCBA[z].devicesName );
   } 
   dynarray_destroy( piCBA );
}

