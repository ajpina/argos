/**
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          openConnction.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021
 *
 * @brief         This function opens a MODBUS TCP connection to a device
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

 #include "privateModbusTcpHeader.h"


int ARGOS_MODBUS_TCP_openConnection( device * D ) {
int j,saveError;
mbTcpControl * P;
    P = ( mbTcpControl * ) D->pluginControl; 
    if ( D->enabled == 'Y' ) {
       j = modbus_connect( P->mbContext );
       if ( j == -1 ) {
             saveError = errno;
             ARGOS_PRINTF("Unable to connect to MODBUS server at  %s : %s\n",P->ipAddress, P->ipPort);
             ARGOS_PRINTF("Error : %d  %s\n", saveError, strerror( saveError ) );
             ARGOS_PRINTF("Marking device %d not connected!\n",D->index);
             ARGOS_PRINTF("Continuing ...\n");
             D->connected = 'N';
       } else {
             D->connected = 'Y';          
       }
    }
    return 0;
 }


