/**
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          init.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021
 *
 * @brief         This is the initiaization routine of a plugin to support 
 *                the MODBUS protocol over TCP
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

 #include "privateModbusTcpHeader.h"
 
 extern int    ARGOS_MODBUS_TCP_openConnection( device * D );
 extern int    ARGOS_MODBUS_TCP_closeConnection( device * D );
 extern void * ARGOS_MODBUS_TCP_manageDevice( void * V );
 extern int    ARGOS_MODBUS_TCP_destroyDevice( device * D );
 
 /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * These are the data types supported by this driver.
 * Make sure that this array is sorted into alphabetical order
 * since we use it with the bsearch utility function.
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

int      numberSupportedDataTypes = 2;

mbTypes  supportedDataTypes[] = {
                                  { "SHORT", SHORT_TYPE },
                                  { "BIT",   BIT_TYPE },
                                };

int      numberSupportedTargetTypes = 4;

mbTypes  supportedTargetTypes[] = {
                                    { "OCOILS", OCOIL_TARGET },
                                    { "ICOILS", ICOIL_TARGET },
                                    { "IREG",   IREG_TARGET  },
                                    { "HREG",   HREG_TARGET  },
                                  };
                                  
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Limit Values from the MODBUS library
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

int      MODBUS_LIMITS[]        = {
                                     0,                         // UNKNOWN
                                     MODBUS_MAX_READ_BITS,      // OCOIL
                                     MODBUS_MAX_WRITE_BITS,     // ICOIL
                                     MODBUS_MAX_READ_REGISTERS, // IREG
                                     MODBUS_MAX_WRITE_REGISTERS // HREG
                                  };


int dataTypeComparator( const void * p1, const void * p2 ) {
    return strcmp( * ( char * const * ) p1, * ( char * const * ) p2 );
}

int evalRegComparator( const void * p1, const void * p2 ) {
const evalReg *r1, *r2;
    r1 = ( evalReg * )p1;
    r2 = ( evalReg * )p2;
    return ( r1->addressInDevice > r2->addressInDevice );
}

void makeReadingPlan( mbTcpControl * T ) {
int       i,
          startSpan,
          currentMax,
          currentTarget,
          firstAddr;
readPlan  wkPlan;

   i             = 0;
   currentTarget = T->expandedRegArray[i].targetType;
   currentMax    = MODBUS_LIMITS[currentTarget];
   firstAddr     = T->expandedRegArray[i].addressInDevice;
   startSpan     = i;
   
   while ( i < dynarray_length( T->expandedRegArray ) ) {
     while ( ( currentTarget == T->expandedRegArray[i].targetType )          &&  // Same Target
             ( ( T->expandedRegArray[i].addressInDevice - firstAddr ) == 
               ( i - startSpan )                                           ) &&  // Contiguous
             ( ( i - startSpan ) < currentMax )                                  // Not too many
           ) { i++; }  
   
     wkPlan.startAddr      = ( ( firstAddr - 1 ) % 100000 ); 
     wkPlan.dataType       = currentTarget;
     wkPlan.numberToAccess = i - startSpan;
     dynarray_push( T->readPlanArray, wkPlan );

   
     startSpan     = i;
     firstAddr     = T->expandedRegArray[i].addressInDevice; 
     currentTarget = T->expandedRegArray[i].targetType;
     currentMax    = MODBUS_LIMITS[currentTarget];
   }  
}


 int ARGOS_MODBUS_TCP_init( device * * cD, char * cN ) {
  /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  *
  * cD is the array of configuredDevices ( pointer to pointer to ... )
  * cN is the name of the configuration file
  *
  * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  */
 device         wkDevice;
 mbTcpControl * TCB;
 int            j,z,saveError;

 dataReg        workReg;
 evalReg        eR;

 char           wkString[64];
 char         * wkJunk;
 mbTypes      * dT;
 int            numberDevices = 0;
 int            numberPoints  = 0;
 

 /* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * ENSURE that the arrays containing the supported data types and
 * the supported target types are in alphabetical order. Failure
 * to do this will generate program errors and malfunctions
 * which are VERY hard to track down.
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   qsort( supportedDataTypes,
          numberSupportedDataTypes,
          sizeof( mbTypes ),
          dataTypeComparator );

   qsort( supportedTargetTypes,
          numberSupportedTargetTypes,
          sizeof( mbTypes ),
          dataTypeComparator );
 /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  * These values are constant for all the devices we will configure 
  * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  */ 
    wkDevice.openConnection   = ARGOS_MODBUS_TCP_openConnection;
    wkDevice.closeConnection  = ARGOS_MODBUS_TCP_closeConnection;
    wkDevice.manageDevice     = ARGOS_MODBUS_TCP_manageDevice;
    wkDevice.destroyDevice    = ARGOS_MODBUS_TCP_destroyDevice;
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * Process the configuration file ( cN ) which describes the devices
 * we are to control ...
 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Make sure the configuration file is valid JSON
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
   if ( access( cN, F_OK ) == -1 ) {  /* File does not exist  */
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Look for a compressed version of the file
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
      wkJunk = ( char * ) calloc( strlen( cN ) + 10, 1 );
      strcpy( wkJunk, cN );
      strcat( wkJunk, ".sqzd" );
      free( cN );
      cN = strdup( wkJunk );
      free( wkJunk );
      if ( access( cN, F_OK ) == -1 ) {  /* File does not exist  */
        ARGOS_PRINTF("The configuration file does not exist!\nTerminating ...\n");
        return 1;
      }
   }
   if ( ! json_validator( cN ) ) {
      ARGOS_PRINTF("The configuration file you have supplied is NOT valid json syntax!\n");
      ARGOS_PRINTF("Correct the errors before re-trying ...\n");
      return 1;
   }
   
   json_tokenizer( cN );
   json_buildDom( );

   numberDevices      = json_array_length("$.devices");
   for( int k = 0; k < numberDevices; k++ ) {
       TCB  = wkDevice.pluginControl
            = calloc( 1, sizeof( mbTcpControl ) );
       wkDevice.connected = 'N';
       wkDevice.enabled   = 'N';

       sprintf( wkString, "$.devices[%d].name",k );
       wkDevice.deviceName        = json_extract( wkString );

       sprintf( wkString, "$.devices[%d].enabled",k );
       wkDevice.enabled           = json_extract( wkString )[0];

       sprintf( wkString, "$.devices[%d].samplerate",k );
       wkDevice.sampleFrequency   = atoi( json_extract( wkString ) );

       sprintf( wkString, "$.devices[%d].communication.address",k );
       TCB->ipAddress             = strdup( json_extract( wkString ) );

       sprintf( wkString, "$.devices[%d].communication.port",k );
       TCB->ipPort                = strdup( json_extract( wkString ) );

       sprintf( wkString,"$.devices[%d].datapoints",k );
       numberPoints                 = json_array_length( wkString );
       TCB->regArray                = dynarray_create( dataReg  );
       TCB->expandedRegArray        = dynarray_create( evalReg  );
       TCB->readPlanArray           = dynarray_create( readPlan );

       if ( numberPoints == 0 ) {
          ARGOS_PRINTF("The number of points you have specified is 0!\n");
          ARGOS_PRINTF("With nothing to monitor there is little sense in continuing!\n");
          ARGOS_PRINTF("Disabling the device ... \n");
          wkDevice.enabled = 'N';
          return 1;
       } else {

          workReg.addressInDevice = NULL;
          workReg.dataType        =    0;
          workReg.howMany         =    0;
          workReg.targetType      =    0;
          workReg.writeable       =  'N';

          for( int i = 0; i < numberPoints; i++ ) {
/* Modbus Register Address */
             sprintf( wkString, "$.devices[%d].datapoints[%d].address",k,i );
             workReg.addressInDevice = strdup( json_extract( wkString ) );
/* Data Type  */
             sprintf( wkString, "$.devices[%d].datapoints[%d].datatype",k,i );
             wkJunk = strdup( json_extract( wkString ) );
             dT = ( mbTypes * )bsearch( ( void * ) &wkJunk,
                                        supportedDataTypes,
                                        numberSupportedDataTypes,
                                        sizeof( mbTypes ),
                                        dataTypeComparator );
             if ( dT == NULL ) {
                ARGOS_PRINTF("The JSON file for device[%d] indicates a DATA type of %s\n",
                       i,wkJunk);
                ARGOS_PRINTF("This DATA type is not supported!\n");
                wkDevice.enabled = 'N';
                workReg.dataType = 0;
             } else {
                workReg.dataType = dT->enumValue;
             }
/* Quantity   */
             sprintf( wkString, "$.devices[%d].datapoints[%d].quantity",k,i );
             workReg.howMany = atoi( json_extract( wkString ) );
/* Target Type  */
             sprintf( wkString, "$.devices[%d].datapoints[%d].targettype",k,i );
             wkJunk = strdup( json_extract( wkString ) );
             dT = ( mbTypes * )bsearch( ( void * ) &wkJunk,
                                        supportedTargetTypes,
                                        numberSupportedTargetTypes,
                                        sizeof( mbTypes ),
                                        dataTypeComparator );
             if ( dT == NULL ) {
                ARGOS_PRINTF("The JSON file for device[%d] indicates a TARGET type of %s\n",
                       i,wkJunk);
                ARGOS_PRINTF("This TARGET type is unknown or not supported!\n");
                wkDevice.enabled = 'N';
                workReg.targetType = 0;
             } else {
                workReg.targetType = dT->enumValue;
             }
/* Writeable    */
             sprintf( wkString, "$.devices[%d].datapoints[%d].writeable",k,i );
             workReg.writeable = json_extract( wkString )[0];

             dynarray_push( TCB->regArray, workReg );

          }
       }
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * We have processed the specifications now for 1 device
 * but before we add it to the array of configured devices
 * we need to expand the register groups ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
       for( j = 0; j < dynarray_length( TCB->regArray ); j++ ) {
        for( z = 0; z < TCB->regArray[j].howMany;z++) {
           eR.addressInDevice = atoi(TCB->regArray[j].addressInDevice) + z;
           eR.value.dataType  = TCB->regArray[j].dataType;
           eR.targetType      = TCB->regArray[j].targetType;
           eR.value.value.L   = 0L;
           dynarray_push( TCB->expandedRegArray, eR );
        }
       }
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Sort them into the proper order
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
       qsort( TCB->expandedRegArray,
              dynarray_length( TCB->expandedRegArray ),
              sizeof( eR ), evalRegComparator );
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Now we are going to make up a "register reading plan" so that when we
 * get advised to refresh the device data, we can do so in the most 
 * efficient manner possible ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
       makeReadingPlan( TCB ); 
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Tell the driver how much shared memory we will need ...
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
       wkDevice.shm_size_required = dynarray_length( TCB->expandedRegArray ) * 
                                    sizeof( evalReg );
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * And finally get ready to handle the MODBUS protocol
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */              
       TCB->mbContext            = modbus_new_tcp_pi( TCB->ipAddress, TCB->ipPort );
       if ( TCB->mbContext == NULL ) {
           saveError = errno;
           ARGOS_PRINTF("Error creating MODBUS context!\nError %d <%s>\n",
                  saveError, strerror( saveError ) );
           ARGOS_PRINTF("Device <%s> disabled!",wkDevice.deviceName);
           wkDevice.enabled = 'N';
       }
/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * This device is finally ready so put it into the array and get the next 
 * device setup
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */
       dynarray_push( *cD, wkDevice );
   }  /* End For  */
   json_finalize();
   return 0;
 }
