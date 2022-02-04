/**
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * @file          closeCOnnection.c
 * @author        Wayne Collins
 *          ---   This file is part of the ARGOS_SCADA system   ---
 *                        Copyright (C) 2021
 *
 * @brief         This routine closes a MODBUS TCP connection
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

 #include "privateModbusTcpHeader.h"

int ARGOS_MODBUS_TCP_closeConnection( device * D ) {
mbTcpControl * P;
  
    if ( ( D->enabled == 'Y' ) && ( D->connected == 'Y' ) ) {
      P = ( mbTcpControl * )D->pluginControl;
      modbus_flush( P->mbContext );
      modbus_close( P->mbContext );
      D->connected = 'N';
    }
    return 0;
 }

